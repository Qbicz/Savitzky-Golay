 %Example 1- Read a signal from PhysioNet's Remote server:
[tm, signal]=rdsamp('mitdb/100',[],10000); % TODO: whole signal has 30 minutes, find a way to process it quickly
plot(tm,signal(:,1))