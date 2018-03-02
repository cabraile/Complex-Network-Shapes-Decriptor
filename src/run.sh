TEST_NAME="test_sd";

g++ -std=c++11 "test/${TEST_NAME}.cpp" -o "bin/${TEST_NAME}"  && {
  bin/${TEST_NAME}
}
