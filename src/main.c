/* main.c
 *
 * Copyright 2022 Matt Sturgeon
 *
 * This program is free software: you
 * can redistribute it and/or modify it
 * under the terms of the GNU General
 * Public License as published by the
 * Free Software Foundation, either
 * version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the
 * hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even
 * the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU
 * General Public License for more
 * details.
 *
 * You should have received a copy of
 * the GNU General Public License along
 * with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <glib.h>
#include <stdlib.h>

#include "line-wrapper.h"

#define TEST_STRING                                                                                \
  "\n\nTest arguments are cool you "                                                               \
  "know, especially when the line gets "                                                           \
  "really "                                                                                        \
  "reaaalllllllyreallllllyreallllllylon"                                                           \
  "glikethisoneisstartingtoallyandlonga"                                                           \
  "ndlongandlongandlong,and then we "                                                              \
  "should be on a new-line that itself "                                                           \
  "might not wrap yet...\nI "                                                                      \
  "wonder\nhow it will\n\n\nbehave "                                                               \
  "with a bunch of \n  random newlines "                                                           \
  "  \n   and spaces... \n\n"

gint main(gint argc, gchar* argv[])
{
  g_autoptr(Lines) wrapped_lines = NULL;
  g_autoptr(GOptionContext) context = NULL;
  g_autoptr(GError) error = NULL;
  gboolean version = FALSE;
  gsize line_length = 80;
  gboolean hyphons_if_wrap_impossible = FALSE;
  GOptionEntry main_entries[] = {
      {"line-length", 'l', 0, G_OPTION_ARG_NONE, &line_length, "Show program version"},
      {"use-hyphens",
       'f',
       0,
       G_OPTION_ARG_NONE,
       &hyphons_if_wrap_impossible,
       "Show program version"},
      {"version", 0, 0, G_OPTION_ARG_NONE, &version, "Show program version"},
      {NULL},
  };
  context = g_option_context_new("- my command line tool");
  g_option_context_add_main_entries(context, main_entries, NULL);
  if (!g_option_context_parse(context, &argc, &argv, &error))
  {
    g_printerr("%s\n", error->message);
    return EXIT_FAILURE;
  }
  if (version)
  {
    g_printerr("%s\n", PACKAGE_VERSION);
    return EXIT_SUCCESS;
  }
  gchar* input = NULL;
  if (argc < 2)
  {
    g_printerr("No input provided, using default test "
               "string\n");
    input = TEST_STRING;
  } else
  {
    input = argv[1];
  }
  g_print("Wrapping \"%s\"...\n", input);
  wrapped_lines = wrap_lines(input, line_length, hyphons_if_wrap_impossible);
  g_print("Wrapped into %d lines:\n", wrapped_lines->len);
  for (guint i = 0; i < wrapped_lines->len; i++)
  {
    gchar* line = wrapped_lines->array[i];
    g_print("[%d]: \"%s\"\n", i, line);
  }
  return EXIT_SUCCESS;
}
