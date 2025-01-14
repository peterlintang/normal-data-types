% 读取视频文件
video = VideoReader('video.mp4');
 
% 创建一个窗口显示视频帧
videoPlayer = videoinput('winvideo', 1);
set(videoPlayer, 'Position', [100, 100, 640, 480]);
 
% 开始播放视频
preview(videoPlayer, video);
 
% 循环直到用户停止
while ~isDone(videoPlayer)
    % 读取下一帧
    frame = step(videoPlayer);
    
    % 将帧转换为灰度
    grayFrame = rgb2gray(frame);
    
    % 使用Canny算法进行边缘检测
    edgeFrame = edge(grayFrame, 'Canny');
    
    % 显示边缘检测结果
    step(videoPlayer, edgeFrame);
    
    % 暂停，等待用户响应
    pause(0.1);
end
 
% 关闭视频播放器
close(videoPlayer);
