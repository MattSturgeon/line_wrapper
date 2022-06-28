/* main.c
 *
 * Copyright 2022 Matt Sturgeon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "line_wrapper-config.h"
#include "line-wrapper.h"

#include <glib.h>
#include <stdlib.h>



gint
main (gint   argc,
      gchar *argv[])
{
  g_autoptr(GPtrArray) wrapped_lines = NULL;
  g_autoptr(GOptionContext) context = NULL;
  g_autoptr(GError) error = NULL;
  gboolean version = FALSE;
  gsize line_length = 80;
  gboolean hyphons_if_wrap_impossible = FALSE;
  GOptionEntry main_entries[] = {
    { "line-length", 'l', 0, G_OPTION_ARG_NONE, &line_length, "Show program version" },
    { "use-hyphens", 'f', 0, G_OPTION_ARG_NONE, &hyphons_if_wrap_impossible, "Show program version" },
    { "version", 0, 0, G_OPTION_ARG_NONE, &version, "Show program version" },
    { NULL }
  };

  context = g_option_context_new ("- my command line tool");
  g_option_context_add_main_entries (context, main_entries, NULL);

  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_printerr ("%s\n", error->message);
      return EXIT_FAILURE;
    }

  if (version)
    {
      g_printerr ("%s\n", PACKAGE_VERSION);
      return EXIT_SUCCESS;
    }

  if (argc < 2)
    {
      g_printerr ("No input provided\n");
      return EXIT_FAILURE;
    }

  g_print("Wrapping \"%s\"...\n", argv[1]);
  g_print("\n\n[STARTING]\n");

  wrapped_lines = wrap_lines (argv[1], line_length, hyphons_if_wrap_impossible);

  g_print("\n\n[FINISHED]\n");
  g_print("Wrapped into %d lines:\n", wrapped_lines->len);

  for (guint i = 0; i < wrapped_lines->len; i++)
    {
      gchar *line = g_ptr_array_index (wrapped_lines, i);
      g_print("[%d]: \"%s\"\n", i, line);
    }



  return EXIT_SUCCESS;
}
