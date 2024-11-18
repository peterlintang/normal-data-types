videoFReader = vision.VideoFileReader('video.mp4');
videoPlayer = vision.VideoPlayer;
while ~isDone(videoFReader)
	videoFrame = step(videoFReader);
    	% 将帧转换为灰度
    	grayFrame = rgb2gray(videoFrame);
    
    	% 使用Canny算法进行边缘检测
    	edgeFrame = edge(grayFrame, 'Canny');
    
    	% 显示边缘检测结果
    	step(videoPlayer, edgeFrame);
	%step(videoPlayer, videoFrame);
	pause(0.05);
end
release(videoPlayer);
release(videoFReader);
