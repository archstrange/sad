echo '
Vector "SadArray"
T "struct Sad"
pre_decl "struct Sad;"
' | ohce Vector.h.ohce > SadArray.h

echo '
Vector "SadArray"
T "struct Sad"
inc "#include \"sad.h\""
' | ohce Vector.c.ohce > SadArray.c

echo '
Vector "SadDict"
T "struct SadEntry"
pre_decl "struct SadEntry;"
' | ohce Vector.h.ohce > SadDict.h

echo '
Vector "SadDict"
T "struct SadEntry"
inc "#include \"sad.h\""
' | ohce Vector.c.ohce > SadDict.c
