# How to play btl speaker in NUC972(MPU) with NAU8822(codec)

### Description
Simple AUDIO PLAY GUIDELINE

### AUDIO FILE
|N|Name|Description|
|---|---|---|
|1||apple_mono.wav|wav format|
|2|Output Image data format|RGB888|

### LINKS RELATED TO AUDIO FILES

[AUDIO FILE GENERATION SITE](https://www.texttospeechfree.com/)
[AUDIO FILE EDITOR DOWNLOAD LINK(FREEWARE)](https://www.audacityteam.org/)

![image](https://github.com/keikkang/NUC972_NAU8825_BTL_OUTPUT/assets/108905975/f491cde6-13f2-4a6a-b6f9-226863a01f11)
![image](https://github.com/keikkang/NUC972_NAU8825_BTL_OUTPUT/assets/108905975/6ceeaf7d-b50c-466d-870f-7fc9cac5bfd6)
![image](https://github.com/keikkang/NUC972_NAU8825_BTL_OUTPUT/assets/108905975/b84833b0-0566-48af-9b79-4c439c00710a)




### Enviorment
|N|Name|Description|Note|
|---|---|---|---|
|1|ARM MDK|Keil uVision Ver. 5.34.0.0|IDE|
|2|NUC970_NonOS_BSP|2023_02_15 Released|[Link](https://github.com/OpenNuvoton/N9H26_NonOS_BSP)|
|3|LCD Driver|hx8282 +hx8696|Sync LCD Driver|
|4|Original Image|kakao|.png|
|5|Conver Image|KAKAO_RGB565|.dat|

### Output Image
![KakaoTalk_20230515_174227398](https://github.com/keikkang/1024x600_TFT_LCD_N9H26K5/assets/108905975/d4652fc6-54aa-489e-b6e6-91a38c63ab02)

### Debug Image
![image](https://github.com/keikkang/1024x600_TFT_LCD_N9H26K5/assets/108905975/b75aa7ce-f86f-4a75-8cc9-1ee1210ff9b7)

### How to convert Image to C array 
[Open source image converter](https://notisrac.github.io/FileToCArray/)
![image](https://github.com/keikkang/1024x600_TFT_LCD_N9H26K5/assets/108905975/2ef665aa-dfcb-45ea-9b75-64bf9a68e153)

### Turbo Writer Setting
![image](https://github.com/keikkang/1024x600_TFT_LCD_N9H26K5/assets/108905975/3d6ba7d8-4047-4e20-8908-95d2b884c25d)
|N|Name|Description|Note|
|---|---|---|---|
|1|N9H26_NANDLoader_240MHz_Fast.bin|for NAND flash booting|
|2|VPOST.bin|excute code|
