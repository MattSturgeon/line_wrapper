#include <glib.h>
#include <locale.h>

#include "config.h"
#include "../src/line-wrapper.h"

#define A_REALLY_LONG_LINE "A veryveryverylonglinewith_no_spacesshouldonlywrapifhyphonateisenabled don't you think"
#define OVER_ONE_LINE "This example will probably be around 100 chars long by the time I run out of example text to write..."

typedef struct {
  gsize line_len;
  gboolean hyphonate;
  gchar *input;
  gchar **output;
  gsize output_len;
} TestData;

static gchar *copy_string(const gchar *source)
{
  gsize length = strlen(source);
  gchar *dest = g_malloc(sizeof(gchar) * (length + 1));
  strncpy(dest, source, length + 1);
  dest[length] = '\0';
  return dest;
}

static TestData *build_test_data(gsize line_len, gboolean hyphonate, gchar *input, gsize output_len, gchar *output[])
{
  TestData *test_data = g_malloc(sizeof(TestData));

  test_data->line_len = line_len;
  test_data->hyphonate = hyphonate;
  test_data->output_len = output_len;

  test_data->input = copy_string(input);

  test_data->output = g_malloc(sizeof(gchar*) * output_len);
  for (gsize i = 0; i < output_len; i++)
  {
    test_data->output[i] = copy_string(output[i]);
  }

  return test_data;
}

static void free_test_data(const TestData *test_data)
{
  for (gsize i = 0; i < test_data->output_len; i++)
  {
    g_free(test_data->output[i]);
  }

  g_free(test_data->output);
  g_free(test_data->input);

  g_free(test_data);
}

static void basic_test_with_data(gconstpointer d)
{
  const TestData *test_data = d;

  const GPtrArray *lines = wrap_lines(test_data->input, test_data->line_len, test_data->hyphonate);

  g_assert_cmpuint(test_data->output_len, ==, lines->len);

  for (gsize i = 0; i < lines->len; i++)
  {
    g_assert_cmpstr(test_data->output[i], ==, g_ptr_array_index(lines, i));
  }

  free_test_data(test_data);
}

gint main (gint argc, gchar *argv[])
{
  TestData *test_data;

  setlocale (LC_ALL, "");

  g_test_init (&argc, &argv, NULL);

  test_data = build_test_data(strlen(OVER_ONE_LINE), FALSE, OVER_ONE_LINE, 1, (const char*[]){
    OVER_ONE_LINE,
  });
  g_test_add_data_func("/line-wrapper/test-no-break", test_data, basic_test_with_data);

  test_data = build_test_data(80, FALSE, OVER_ONE_LINE, 2, (const char*[]){
    "This example will probably be around 100 chars long by the time I run out of",
    "example text to write...",
  });
  g_test_add_data_func("/line-wrapper/test-break-before-80", test_data, basic_test_with_data);

  test_data = build_test_data(20, FALSE, OVER_ONE_LINE, 6, (const char*[]){
    "This example will",
    "probably be around",
    "100 chars long by",
    "the time I run out",
    "of example text to",
    "write...",
  });
  g_test_add_data_func("/line-wrapper/test-break-before-20", test_data, basic_test_with_data);

  test_data = build_test_data(20, FALSE, A_REALLY_LONG_LINE, 3, (const char*[]){
    "A",
    "veryveryverylonglinewith_no_spacesshouldonlywrapifhyphonateisenabled",
    "don't you think",
  });
  g_test_add_data_func("/line-wrapper/test-dont-hyphonate", test_data, basic_test_with_data);

  test_data = build_test_data(20, TRUE, A_REALLY_LONG_LINE, 6, (const char*[]){
    "A",
    "veryveryverylonglin-",
    "ewith_no_spacesshou-",
    "ldonlywrapifhyphona-",
    "teisenabled don't",
    "you think",
  });
  g_test_add_data_func("/line-wrapper/test-do-hyphonate", test_data, basic_test_with_data);

  return g_test_run ();
}