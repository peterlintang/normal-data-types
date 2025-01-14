% 读取视频文件
videoFile = 'video.mp4';
reader = vision.VideoFileReader(videoFile);
 
% 加载预训练的MobileNet-V2模型（用于目标检测）
net = vision.CascadeObjectDetector('MobileNetV2ObjectDetector', 'ExecutionEnvironment', 'cpu');
 
% 读取视频的每一帧，进行目标检测，并在帧上显示检测结果
while hasFrame(reader)
    frame = readFrame(reader);
    bboxes = step(net, frame);
    visualizeObjectDetctions(frame, bboxes, 'Color', 'yellow'); % 显示检测结果
    pause(0.1); % 暂停一段时间以控制播放速度
end
