% Example 1- Read ECG signal from PhysioNet's Remote server:
[tm, signal]=rdsamp('mitdb/117',[],100000); % TODO: whole signal has 30 minutes, find a way to process it quickly
%plot(tm,signal(:,1))

% write to text file
fileID = fopen('mitdb117.txt', 'wb');
table = [tm signal]';
fprintf(fileID, 'Time\tMLII [mV]\tV5 [mV]\n');
fprintf(fileID, '%04.3f\t%0.3f\t%0.3f\n', table);
fclose(fileID);
