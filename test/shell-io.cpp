#include "gtest/gtest.h"

extern "C" {
#include "../shell/shell.h"
#include "../util/util.h"
#include "../shell/shell-io.h"
#include "../freescaleK70/io.h"
}

#include <stdio.h>
#include "../third-party/fff.h"
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(int, myCreate, const char *);
FAKE_VALUE_FUNC(Stream *, myFopen, const char *);

class ShellIOTest : public ::testing::Test {
    protected:

  int fds[2];
  FILE *ostrm;
  FILE *istrm;

  Stream *test_stream;
  Stream ts;

  static const int output_string_length = 256;
  char output_string[output_string_length];
  char *output_ptr;

  // You can remove any or all of the following functions if its body
  // is empty.


  ShellIOTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ShellIOTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    RESET_FAKE(myCreate);
    RESET_FAKE(myFopen);

    FFF_RESET_HISTORY();

    test_stream = &ts;

    output_string[0] = '\0';
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  void OpenStreams(void) {
    pipe(fds);
    ostrm = fdopen(fds[1], "w");
    istrm = fdopen(fds[0], "r");
  }

  char *SendInput(const char *input) {
    OpenStreams();
    fputs(input, ostrm);
    fclose(ostrm);

    char *buf = read_input(istrm);
    fclose(istrm);
    return buf;
  }
};

TEST_F(ShellIOTest, Create) {
    int result;
    int argc = 2;
    const char *args[] = {"create", "/dev/fs/data"};
    char **argv = new_array_of_strings(argc, args);

    result = cmd_create(argc, argv, ostrm);
    delete_array_of_strings(argc, argv);

    EXPECT_EQ(SUCCESS, result);
    EXPECT_EQ(1, myCreate_fake.call_count);
}

TEST_F(ShellIOTest, CreateError) {
    int argc = 2;
    const char *args[] = {"create", "invalid_filename"};
    char **argv = new_array_of_strings(argc, args);

    OpenStreams();
    myCreate_fake.return_val = CANNOT_CREATE_FILE;
    cmd_create(argc, argv, ostrm);
    fclose(ostrm);
    delete_array_of_strings(argc, argv);
    fgets(output_string, output_string_length, istrm);
    fclose(istrm);

    EXPECT_STREQ("create: error creating file\n", output_string);
}

TEST_F(ShellIOTest, Fopen) {
    int result;
    int argc = 2;
    const char *args[] = {"fopen", "/dev/fs/data"};
    char **argv = new_array_of_strings(argc, args);

    OpenStreams();

    myFopen_fake.return_val = test_stream;
    result = cmd_fopen(argc, argv, ostrm);
    fclose(ostrm);
    delete_array_of_strings(argc, argv);
    fgets(output_string, output_string_length, istrm);

    EXPECT_EQ(SUCCESS, result);
    EXPECT_EQ(1, myFopen_fake.call_count);
    EXPECT_STREQ("file opened with stream ID:\n", output_string);
}

TEST_F(ShellIOTest, FopenError) {
    int result;
    int argc = 2;
    const char *args[] = {"fopen", "/dev/fs/data"};
    char **argv = new_array_of_strings(argc, args);

    OpenStreams();
    myFopen_fake.return_val = NULL_STREAM;
    result = cmd_fopen(argc, argv, ostrm);
    fclose(ostrm);
    delete_array_of_strings(argc, argv);
    fgets(output_string, output_string_length, istrm);
    fclose(istrm);

    EXPECT_EQ(CANNOT_OPEN_FILE, result);
    EXPECT_EQ(1, myFopen_fake.call_count);
    EXPECT_STREQ("fopen: error opening file\n", output_string);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
