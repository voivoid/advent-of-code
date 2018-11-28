add_executable(AocApp main.cpp
                      solve_problem.h solve_problem.cpp
                      parse_cmd_line.h parse_cmd_line.cpp
                      list_problems.h list_problems.cpp)
target_include_directories(AocApp PRIVATE app)
target_link_libraries(AocApp PRIVATE AocProblems Boost::boost Boost::program_options)
