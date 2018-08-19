add_executable(gh-search gh-search.cpp)
target_link_libraries(gh-search nxxm::gh)

add_executable(gh-branches gh-branches.cpp)
target_link_libraries(gh-branches nxxm::gh)

add_executable(gh-repos gh-repos.cpp)
target_link_libraries(gh-repos nxxm::gh)
