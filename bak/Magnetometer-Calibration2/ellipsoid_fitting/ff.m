% 创建一个 VideoReader 对象来读取视频文件
videoFile = 'video.mp4';
videoReader = VideoReader(videoFile);
 
% 创建一个 VideoPlayer 对象来播放视频
videoPlayer = VideoPlayer(videoReader);
 
% 播放视频
play(videoPlayer);
