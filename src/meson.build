vec_h_ohce = '../../cutils/src/Vector/Vector.h.ohce'
vec_c_ohce = '../../cutils/src/Vector/Vector.c.ohce'

SadArray_h = custom_target(
  'SadArray.h',
  output: 'SadArray.h',
  input: 'SadArray.sad',
  command: [ohce, vec_h_ohce, '@OUTPUT@', '@INPUT@'],
  install: true,
  install_dir: 'include/sad',
)
SadArray_c = custom_target(
  'SadArray.c',
  output: 'SadArray.c',
  input: 'SadArray.sad',
  command: [ohce, vec_c_ohce, '@OUTPUT@', '@INPUT@'],
)

dict_h_ohce = '../../cutils/src/Dict/Dict.h.ohce'
dict_c_ohce = '../../cutils/src/Dict/Dict.c.ohce'

SadDict_h = custom_target(
  'SadDict.h',
  output: 'SadDict.h',
  input: 'SadDict.sad',
  command: [ ohce, dict_h_ohce, '@OUTPUT@', '@INPUT@' ],
  install: true,
  install_dir: 'include/sad',
)
SadDict_c = custom_target(
  'SadDict.c',
  output: 'SadDict.c',
  input: 'SadDict.sad',
  command: [ ohce, dict_c_ohce, '@OUTPUT@', '@INPUT@' ],
)

sad_header = [
  'sad.h',
  'read.h',
  'write.h',
]

sad_src = [
  'read.c',
  'write.c',
  SadArray_h,
  SadArray_c,
  SadDict_h,
  SadDict_c,
]

sad = static_library(
  'sad',
  sad_src,
  dependencies: [ libcutils ],
  install: true
)

install_headers(sad_header, subdir: 'sad')
