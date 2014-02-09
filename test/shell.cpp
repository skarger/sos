
#include "gtest/gtest.h"

extern "C" {
#include <unistd.h>
#include <stdio.h>
#include "../shell/shell.h"
}

class ShellTest : public ::testing::Test {
    protected:

  // You can remove any or all of the following functions if its body
  // is empty.

  int fds[2];
  FILE *ostrm;
  FILE *istrm;

  ShellTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ShellTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
    pipe(fds);
    ostrm = fdopen(fds[1], "w");
    istrm = fdopen(fds[0], "r");
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    fclose(istrm);
    fclose(ostrm);
  }

};


TEST_F(ShellTest, Prompt) {
    char c, d;

    print_prompt(ostrm);
    fclose(ostrm);

    c = fgetc(istrm);
    d = fgetc(istrm);
    fclose(istrm);

    EXPECT_EQ('$', c);
    EXPECT_EQ(' ', d);
}

TEST_F(ShellTest, InputBuffer) {
    const char *input = "cmd_help";
    fputs(input, ostrm);
    fclose(ostrm);

    char *buf = read_input(istrm);
    fclose(istrm);
    ASSERT_STREQ(input, buf);
}

TEST_F(ShellTest, InputBufferLen) {
    char expected_buf[257];
    char long_buf[512];
    int i;

    for (i = 0; i < 256; i++) {
        expected_buf[i] = 'x';
    }
    expected_buf[256] = '\0';

    for (i = 0; i < 512; i++) {
        long_buf[i] = 'x';
    }

    fputs(long_buf, ostrm);
    fclose(ostrm);

    char *buf = read_input(istrm);
    fclose(istrm);
    EXPECT_STREQ(expected_buf, buf);
}

TEST_F(ShellTest, InputBufferLine) {
    const char expected_buf[] = "12345\n";
    const char two_line_buf[] = "12345\n6789";

    fputs(two_line_buf, ostrm);
    fclose(ostrm);

    char *buf = read_input(istrm);
    fclose(istrm);
    EXPECT_STREQ(expected_buf, buf);
}

TEST_F(ShellTest, CountArgs) {
    const char *input = "cmd_help";
    fputs(input, ostrm);
    fclose(ostrm);

    char *buf = read_input(istrm);
    fclose(istrm);

    int ac = count_args(buf);
    EXPECT_EQ(1, ac);
}

TEST_F(ShellTest, ParseNullInput) {
    CommandLine *cl;
    char *buf;
    const char *input = "\n";
    fputs(input, ostrm);
    fclose(ostrm);

    buf = read_input(istrm);
    fclose(istrm);
    cl = parse_input(buf);
    EXPECT_EQ(0, cl->argc);
}

TEST_F(ShellTest, ParseOneArg) {
    CommandLine *cl;
    char *buf;
    const char *input = "cmd_help";
    fputs(input, ostrm);
    fclose(ostrm);

    buf = read_input(istrm);
    fclose(istrm);
    cl = parse_input(buf);
    EXPECT_EQ(1, cl->argc);
    EXPECT_STREQ("cmd_help", cl->argv[0]);
}

TEST_F(ShellTest, ParseTwoArgs) {
    CommandLine *cl;
    char *buf;
    const char *input = "echo hello";
    fputs(input, ostrm);
    fclose(ostrm);

    buf = read_input(istrm);
    fclose(istrm);
    cl = parse_input(buf);
    EXPECT_EQ(2, cl->argc);
    EXPECT_STREQ("echo", cl->argv[0]);
    EXPECT_STREQ("hello", cl->argv[1]);
}

TEST_F(ShellTest, ParseManyArgs) {
    CommandLine *cl;
    char *buf;
    const char *input = "echo     this  is some     input   string";
    fputs(input, ostrm);
    fclose(ostrm);

    buf = read_input(istrm);
    fclose(istrm);
    cl = parse_input(buf);
    EXPECT_EQ(6, cl->argc);
    EXPECT_STREQ("is", cl->argv[2]);
    EXPECT_STREQ("string", cl->argv[5]);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
