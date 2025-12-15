# EEZ UI Component - sources are globbed from ../main/ui/
# This component serves as a dependency container for LVGL integration
# Actual UI sources will be provided by EEZ Studio exports to ../main/ui/

idf_component_register(
  SRCS
  INCLUDE_DIRS "."
  # LVGL will be added as a managed dependency when needed
  # REQUIRES lvgl
)