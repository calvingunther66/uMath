#include "SearchUI.h"
#include <cctype>
#include <cstdio>
#include <cstring>

namespace uMath {

SearchUI::SearchUI() { reset(); }

void SearchUI::reset() {
  std::memset(query_buffer, 0, sizeof(query_buffer));
  filtered_indices.clear();
  selected_formula = nullptr;
  selection_index = -1;
  // Initial filter = show all (or none? User requirement: "Filters as user
  // types") Let's show all initially if query is empty? Or prompt to type?
  // "Query: _" implies empty.
  filter_results();
}

void SearchUI::filter_results() {
  filtered_indices.clear();
  const Formula *all = FormulaBank::getAll();
  size_t count = FormulaBank::getCount();

  bool empty_query = (std::strlen(query_buffer) == 0);

  for (size_t i = 0; i < count; ++i) {
    // Case-insensitive substring search would be ideal.
    // For C-string on embedded, sticking to strstr (sensitive) or writing a
    // helper. Requirement said "case-insensitive". Implementation:

    bool match = false;
    if (empty_query) {
      match = true;
    } else {
      // Manual case-insensitive substring
      const char *haystack = all[i].name;
      const char *needle = query_buffer;

      // Loop haystack
      for (int h = 0; haystack[h]; ++h) {
        // Check match starting at h
        bool sub_match = true;
        for (int n = 0; needle[n]; ++n) {
          if (!haystack[h + n]) {
            sub_match = false;
            break;
          }
          if (std::tolower(haystack[h + n]) != std::tolower(needle[n])) {
            sub_match = false;
            break;
          }
        }
        if (sub_match) {
          match = true;
          break;
        }
      }
    }

    if (match) {
      filtered_indices.push_back(i);
    }
  }

  // Reset selection if list changed
  if (!filtered_indices.empty())
    selection_index = 0;
  else
    selection_index = -1;
}

void SearchUI::onChar(char c) {
  size_t len = std::strlen(query_buffer);

  if (c == '\b' || c == 0x7F) { // Backspace
    if (len > 0) {
      query_buffer[len - 1] = 0;
      filter_results();
    }
  } else if (c == '\n') { // Enter
    // Select logic handled in update usually, but if onChar is from
    // KeyboardMapper... Let's defer selection to update() via OK button or
    // special processing? Actually Phase 9 defines onChar handling printable.
    // Enter might be handled separately.
  } else if (std::isprint(c)) {
    if (len < sizeof(query_buffer) - 1) {
      query_buffer[len] = c;
      query_buffer[len + 1] = 0;
      filter_results();
    }
  }
}

void SearchUI::update(InputManager &input_mgr) {
  EventType e = input_mgr.poll();
  while (e != EventType::NONE) {
    if (e == EventType::BTN_UP) {
      if (selection_index > 0)
        selection_index--;
    } else if (e == EventType::BTN_DOWN) {
      if (selection_index < (int)filtered_indices.size() - 1)
        selection_index++;
    } else if (e == EventType::BTN_OK) {
      if (selection_index >= 0 &&
          selection_index < (int)filtered_indices.size()) {
        int db_idx = filtered_indices[selection_index];
        selected_formula = &FormulaBank::getAll()[db_idx];
      }
    }
    e = input_mgr.poll();
  }
}

void SearchUI::render(DisplayBuffer &display) {
  display.clear();

  char buf[64];
  snprintf(buf, 64, "[ SEARCH MODE ]");
  display.print(0, 0, buf);

  snprintf(buf, 64, "Query: %s_", query_buffer);
  display.print(1, 0, buf);
  display.print(2, 0, "----------------");

  // List results
  // Show 4 lines
  int start_idx = 0;
  if (selection_index > 3)
    start_idx = selection_index - 3;

  const Formula *all = FormulaBank::getAll();
  for (int i = 0; i < 4; ++i) {
    int idx = start_idx + i;
    if (idx >= (int)filtered_indices.size())
      break;

    int f_id = filtered_indices[idx];
    const char *name = all[f_id].name;

    char line[40];
    // Truncate name
    char name_trunc[20];
    strncpy(name_trunc, name, 19);
    name_trunc[19] = 0;

    if (idx == selection_index) {
      snprintf(line, 40, "> %s", name_trunc);
    } else {
      snprintf(line, 40, "  %s", name_trunc);
    }
    display.print(3 + i, 0, line);
  }
}

} // namespace uMath
