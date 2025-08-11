# Messing around with stb_img_write.h

![](checkerboard.png)
![](nxny.png)
![](hsv_diag.png)
![](hsv_circle.png)

## Build and run
```bash
wget https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image_write.h
```
```bash
gcc -o main main.c -lm && ./main 0 && xdg-open checkerboard.png
```
