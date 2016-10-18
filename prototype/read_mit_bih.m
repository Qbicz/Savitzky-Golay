% Example 1- Read ECG signal from PhysioNet's Remote server:
[tm, signal]=rdsamp('mitdb/101',[],100000); % TODO: whole signal has 30 minutes, find a way to process it quickly
%plot(tm,signal(:,1))

% write to text file
fileID = fopen('../mit-bih-txt/mitdb101.txt', 'wb');
table = [tm signal]';
fprintf(fileID, 'Time,MLII [mV],V5 [mV]\n');
fprintf(fileID, '%04.3f,%0.3f,%0.3f\n', table);
fclose(fileID);
