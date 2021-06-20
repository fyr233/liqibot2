@echo off
set JAVA_BINARY=java
%JAVA_BINARY% -server -Xms512m -Xmx4g -XX:+UseG1GC -XX:MaxGCPauseMillis=100 -jar mcl.jar %*
