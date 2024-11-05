idf_component_register(
    SRCS "DataBridge.cpp" "NvsHandler.cpp"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash
)