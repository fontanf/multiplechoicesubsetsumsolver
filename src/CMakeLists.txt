add_library(MultipleChoiceSubsetSumSolver)
target_sources(MultipleChoiceSubsetSumSolver PRIVATE
    instance.cpp
    instance_builder.cpp
    solution.cpp
    algorithm_formatter.cpp)
target_include_directories(MultipleChoiceSubsetSumSolver PUBLIC
    ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MultipleChoiceSubsetSumSolver PUBLIC
    OptimizationTools::utils)
add_library(MultipleChoiceSubsetSumSolver ALIAS MultipleChoiceSubsetSumSolver)

add_subdirectory(algorithms)

if(MULTIPLECHOICESUBSETSUMSOLVER_BUILD_MAIN)
    add_executable(MultipleChoiceSubsetSumSolver_main)
    target_sources(MultipleChoiceSubsetSumSolver_main PRIVATE
        main.cpp)
    target_link_libraries(MultipleChoiceSubsetSumSolver_main PUBLIC
        MultipleChoiceSubsetSumSolver_dynamic_programming_bellman
        Boost::program_options)
    set_target_properties(MultipleChoiceSubsetSumSolver_main PROPERTIES OUTPUT_NAME "multiplechoicesubsetsumsolver")
    install(TARGETS MultipleChoiceSubsetSumSolver_main)
endif()
