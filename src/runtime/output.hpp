#include <string>
#pragma once

void cout(std::string text, bool flush = true, char end = '\n');

void cout(char** text, bool flush = true, char end = '\n');

void flush();

void clearscreen();
