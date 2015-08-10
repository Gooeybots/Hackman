#ifndef SETUPPROGRAM_HPP
#define SETUPPROGRAM_HPP

/* creates a program and returns it from vertex shader and
 * fragment shader returns 0 if program couldnt be created */
GLuint SetupProgram(const char * filenameVS, const char * filenameFS);

#endif // SETUPPROGRAM_HPP
