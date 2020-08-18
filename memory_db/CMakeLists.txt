add_library(memory_db db.c)

target_link_libraries(memory_db -lsqlite3)

# anyobody that includes this libraries needs to include current source dir
target_include_directories(memory_db
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )

# Install rules
install(TARGETS memory_db DESTINATION lib)
install(FILES db.h DESTINATION include)
