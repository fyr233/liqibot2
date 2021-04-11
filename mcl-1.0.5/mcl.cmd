@echo off
set JAVA_BINARY=java
%JAVA_BINARY% -server -Xms512m -Xmx2g -XX:+UseG1GC -XX:MaxGCPauseMillis=400 -jar mcl.jar %*
