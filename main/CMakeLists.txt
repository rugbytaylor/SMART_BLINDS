idf_component_register(SRCS "main.c" "blinds_SM.c"
                    INCLUDE_DIRS "."
                    REQUIRES freertos button servo esp_pm)
