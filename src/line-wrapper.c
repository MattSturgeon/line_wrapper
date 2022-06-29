#include "line-wrapper.h"

#include <string.h>

gchar *wrap_line(gchar *start, gsize length, gchar *suffix)
{
  gsize suffix_len = strlen(suffix);
  gsize input_len = length - suffix_len;
  gchar *buffer = malloc((length + 1) * sizeof(gchar));
  // FIXME handle error

  g_print("wrap_line: start %p, length %ld, input_len %ld, suffix_len %ld\n",
    start,
    length,
    input_len,
    suffix_len);

  if (input_len > 0)
  {
    gsize bytes = memcpy(buffer, start, input_len * sizeof(gchar));
    g_print("copied %ld bytes\n", bytes);
    // FIXME handle error
  }

  if (suffix_len > 0){
    gsize bytes = memcpy(buffer + input_len, suffix, suffix_len * sizeof(gchar));
    g_print("copied %ld bytes\n", bytes);
    // FIXME handle error
  }

  buffer[length] = '\0';

  return buffer;
}

GPtrArray *wrap_lines(gchar *input, gsize line_length, gboolean hyphons_if_wrap_impossible)
{
  GPtrArray *array;
  gchar *last_wrapped_at;
  gchar *last_whitespace;
  gboolean replace_whitespace;
  gsize current_line_length;

  last_wrapped_at = input;
  last_whitespace = NULL;
  replace_whitespace = FALSE;

  array = g_ptr_array_new();

  for (gsize i = 0; TRUE; i++)
  {
    current_line_length = &input[i] - last_wrapped_at;
    // g_print("[%ld]: current_line_length %ld (max %ld)\n", i, current_line_length, line_length);

    // Check if whitespace
    switch(input[i])
    {
      // TODO add more whitespace
      case '\0':
      case '\n':
      case ' ': {
        last_whitespace = &input[i];
        replace_whitespace = TRUE;
      } break;
      case '-': {
        last_whitespace = &input[i];
        replace_whitespace = FALSE;
      } break;
    }

    if (!input[i] || current_line_length >= line_length)
    {
      gsize final_line_length;
      gchar *wrap_at;
      gchar *line;
      gchar *suffix = "";

      wrap_at = last_whitespace;
      if (wrap_at < last_wrapped_at)
      {
        // No whitespace since last wrap...
        if (hyphons_if_wrap_impossible)
        {
          suffix = "-";
          wrap_at = &input[i];
        }
        else continue;
      }
      
      final_line_length = wrap_at - last_wrapped_at;
      g_print("[%ld] final_line_length %ld, suffix \"%s\"\n", i, final_line_length, suffix);

      line = wrap_line(last_wrapped_at, final_line_length, suffix);
      //FIXME chedck error

      last_wrapped_at = wrap_at;

      if (replace_whitespace)
      {
        last_wrapped_at++;
      }

      g_print("line: \"%s\"\n", line);

      g_free(line);// FIXME move up a scope or two
    }

    if (!input[i])
    {
      // Reached the end of the string
      break;
    }
  }

  return array;
}

