# Project说明

基于QT打造一个H264分析工具

# 功能

1. ID
2. Pos
3. IDC
4. TYPE
5. LEN

# 界面效果

```
+---------------NALU-------------------------+
  ID | POS |  IDC    | TYPE   |  LEN  |
   0 |   0 | HIGHEST |  SPS   |   25  |
   1 |  29 | HIGHEST |  PPS   |   6   |
   2 |  39 | DISPOS  |  SEI   |  648  | 
   3 | 728 | HIGHEST |  IDR   | 14710 |
   0 | 826 | HIGHEST |  SLICE |  6675 |
   0 | 999 | HIGHEST |  SLICE |  7725 |

```