ctype_to_ftype = {
    'bool' : 'logical',
    'int' : 'integer',
    'float' : 'real',
    'double' : 'real' }

def write_dope_sizes(file, rank):
    file.write((
        '#ifndef DOPEY_DOPE_SIZES_H\n'
        '#define DOPEY_DOPE_SIZES_H\n'
        '\n'
        '#define DOPEY_DOPE_MAX_RANK {RANK:d}\n'
        '\n'
        '#endif\n').format(
            RANK=rank)
    )

def fortran_comment(string):
    return '! ' + string

def c_comment(string):
    return '// ' + string

def prepro_comment(string):
    return '#define COMMENT ' + string + '#undef COMMENT\n'

def write_inclusion_guard(file):
    file.write((
        '#ifndef DOPEY_NOT_INCLUDED_DIRECTLY\n'
        '#error "{FILE:s} should not be included directly"\n'
        '#endif\n\n').format(
            FILE=file.name)
    )

def write_generated_warning(file, comment_fn):
    file.write(comment_fn('DO NOT EDIT THIS FILE DIRECTLY!\n'))
    file.write(comment_fn('This file was automatically generated by `generate_dope_interfaces.py`\n'))
    file.write('\n')

def write_fortran_interface(file, rank, ctype):
    file.write('module procedure :: make_dope_{RANK:d}d_c_{TYPE}\n'.format(
        RANK=rank,
        TYPE=ctype)
    )

def write_fortran_definitions(file, rank, ctype):
    if rank > 0:
        file.write((
            'function make_dope_{RANK:d}d_c_{TYPE}(a) result(retval)\n'
            '  use, intrinsic :: iso_c_binding\n'
            '  implicit none\n'
            '  {FTYPE}(c_{TYPE}), dimension({DIM}), intent(in), target :: a\n'
            '  type(dope) :: retval\n'
            '  {FTYPE}(c_{TYPE}), dimension({DIM}), pointer :: p\n'
            '  \n'
            '  interface\n'
            '    subroutine make_dope_{RANK:d}d_{TYPE:s}(d,a) bind(c)\n'
            '      use, intrinsic :: iso_c_binding\n'
            '      import dope\n'
            '      implicit none\n'
            '      type(dope), intent(out) :: d\n'
            '      {FTYPE}(c_{TYPE}), dimension({DIM}), pointer, intent(in) :: a\n'
            '    end subroutine\n'
            '  end interface\n'
            '  \n'
            '  p => a\n'
            '  call make_dope_{RANK:d}d_{TYPE:s}(retval,p)\n'
            'end function\n'
            '\n').format(
                    RANK=rank,
                    TYPE=ctype,
                    FTYPE=ctype_to_ftype[ctype],
                    DIM=','.join([':' for _ in range(rank)]))
        )
    else:
        file.write((
            'function make_dope_{RANK:d}d_c_{TYPE}(a) result(retval)\n'
            '  use, intrinsic :: iso_c_binding\n'
            '  implicit none\n'
            '  {FTYPE}(c_{TYPE}), target :: a\n'
            '  type(dope) :: retval\n'
            '  \n'
            '  interface\n'
            '    subroutine make_dope_{RANK:d}d_{TYPE:s}(d,a) bind(c)\n'
            '      use, intrinsic :: iso_c_binding\n'
            '      import dope\n'
            '      implicit none\n'
            '      type(dope), intent(out) :: d\n'
            '      {FTYPE}(c_{TYPE}), intent(in) :: a\n'
            '    end subroutine\n'
            '  end interface\n'
            '  \n'
            '  call make_dope_{RANK:d}d_{TYPE:s}(retval,a)\n'
            'end function\n'
            '\n').format(
                    RANK=rank,
                    TYPE=ctype,
                    FTYPE=ctype_to_ftype[ctype],
                    DIM=','.join([':' for _ in range(rank)]))
        )

def write_c_definition(file, rank, ctype):
    if rank > 0:
        file.write((
            'void make_dope_{RANK:d}d_{TYPE:s}(dope<{TYPE:s},{RANK:d}>& d, CFI_cdesc_t const& a) {{\n'
            '  d = detail::make_dope<{TYPE:s},{RANK:d}>(a);\n'
            '}}\n'
            '\n').format(
                    RANK=rank,
                    TYPE=ctype)
        )
    else:
        file.write((
            'void make_dope_{RANK:d}d_{TYPE:s}(dope<{TYPE:s},{RANK:d}>& d, {TYPE:s}* const a) {{\n'
            '  d = detail::make_dope0<{TYPE:s}>(a);\n'
            '}}\n'
            '\n').format(
                    RANK=rank,
                    TYPE=ctype)
        )

def write_cpp_type_identifier_trait(file, ctype):
    def maybe_modified_type(ctype):
        result = ctype
        if ctype == 'bool':
            result = 'Bool'
        return result

    file.write((
        'template<>\n'
        'struct type_identifier<{TYPE:s}> {{\n'
        '  constexpr static CFI_type_t value = CFI_type_{MODTYPE:s};\n'
        '}};'
        '\n\n').format(TYPE=ctype,
                       MODTYPE=maybe_modified_type(ctype)))

def main():
    import argparse
    import os
    
    ap = argparse.ArgumentParser()
    
    ap.add_argument('RANK', type=int)
    ap.add_argument('TYPE', nargs='+')
    ap.add_argument('--outdir', type=str, default='.')
    
    args = ap.parse_args()

    if not os.path.isdir(args.outdir):
        os.mkdir(args.outdir)

    with open(os.path.join(args.outdir, 'dope_generated_sizes.h'), 'w') as dope_sizes:
        write_generated_warning(dope_sizes, prepro_comment)
        write_inclusion_guard(dope_sizes)
        write_dope_sizes(dope_sizes, args.RANK)
    
    with open(os.path.join(args.outdir, 'dope_generated_fortran_interfaces.h'), 'w') as fortran_interfaces:
        write_generated_warning(fortran_interfaces, fortran_comment)
        write_inclusion_guard(fortran_interfaces)

        for ctype in args.TYPE:
            for rank in range(0, args.RANK+1):
                write_fortran_interface(fortran_interfaces, rank, ctype)

    with open(os.path.join(args.outdir, 'dope_generated_fortran_definitions.h'), 'w') as fortran_definitions:
        write_generated_warning(fortran_definitions, fortran_comment)

        for ctype in args.TYPE:
            for rank in range(0, args.RANK+1):
                write_fortran_definitions(fortran_definitions, rank, ctype)
    
    with open(os.path.join(args.outdir, 'dope_generated_c_definitions.hpp'), 'w') as c_definitions:
        write_generated_warning(c_definitions, c_comment)
        write_inclusion_guard(c_definitions)

        for ctype in args.TYPE:
            for rank in range(0, args.RANK+1):
                write_c_definition(c_definitions, rank, ctype)

    with open(os.path.join(args.outdir, 'dope_generated_type_identifier_traits.hpp'), 'w') as cpp_type_identifiers:
        write_generated_warning(cpp_type_identifiers, c_comment)
        write_inclusion_guard(cpp_type_identifiers)
        cpp_type_identifiers.write((
            'template<typename T>\n'
            'struct type_identifier {};\n'
            '\n'))
        
        for ctype in args.TYPE:
            write_cpp_type_identifier_trait(cpp_type_identifiers, ctype)

        cpp_type_identifiers.write((
            'template<typename T>\n'
            'constexpr static auto type_identifier_v = type_identifier<T>::value;\n'
            '\n'))
    
if __name__ == "__main__":
    main()
