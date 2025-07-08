# CMake generated Testfile for 
# Source directory: /Users/sergei/Desktop/code/final/search_engine
# Build directory: /Users/sergei/Desktop/code/final/search_engine/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "/Users/sergei/Desktop/code/final/search_engine/build/tests")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/sergei/Desktop/code/final/search_engine/CMakeLists.txt;40;add_test;/Users/sergei/Desktop/code/final/search_engine/CMakeLists.txt;0;")
subdirs("nlohmann_json")
subdirs("_deps/googletest-build")
