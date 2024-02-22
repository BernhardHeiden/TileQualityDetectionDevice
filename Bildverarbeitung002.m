function Bildverarbeitung002
clc
close all
clear all
warning off
cam = webcam(1); %connection to webcam
img = snapshot(cam); %make pic
fname = ['Kamerabild']; %save as Kamerabild % wenn aufsteigende Nummer: fname = ['Image' num2str(i)]; %filename => Image1, Image2...
imwrite(img, fname,'jpeg'); %save as jpg
imshow(img); %show the image
vb = imread("Kamerabild");
subplot(2,2,1) %divides the figure into an m-by-n grid and creates axes in the position specified by p
imshow(vb);
title('Subplot 1: Kamerabild')
%create mask1
gs33 = createMask1(vb);
%figure;
subplot(2,2,2)
imshow(gs33);
title('Subplot 2: Bild mit Maske')
%löcher füllen (create mask1)
ks33 = strel('disk', 15);
ls33 = imdilate(gs33,ks33);
%figure;
subplot(2,2,3)
imshow(ls33);
title('Subplot 3: Bild mit gefüllten Löchern')
%Label connected components in 2-D binary image (create mask1)
a33 = bwlabel(ls33);
%disp(a33);
%remove salt and pepper noise
b33 = medfilt2(a33);
subplot(2,2,4)
imshow(b33);
title('Subplot 4: Bild ohne noises')
%count black pixel
bw33 = imbinarize(b33); %make a binary image
blackPixels1 = bw33== 1;
blackVb3 = sum(blackPixels1(:)); %zählt 1en (also schwarzanteil)
display(blackVb3);
%count white pixel
bww3 = imbinarize(b33);
whitePixels = bww3 == 0; % count 0 (weiße Pixel)
whiteVb3 = sum(whitePixels(:));
display(whiteVb3);
%display solution
if (blackVb3 > 145518 && whiteVb3 > 1493108)
disp('Correct one');
c = imread("correct.jpg");
figure;
subplot(1,1,1)
imshow(c);
title('Subplot 5: Bewertung')
else
disp('Incorrect one');
i = imread("incorrect.jpg");
figure;
subplot(1,1,1)
imshow(i);
title('Subplot 5: Bewertung')
%arduino send number to arduino
end
save("variable.mat","blackVb3","whiteVb3")
end