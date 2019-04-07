#include "AoC/2018/problem_07.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/algorithm/max.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/range/primitives.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/set_algorithm.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/topological_sort.hpp"
#include "boost/numeric/conversion/cast.hpp"


#include <algorithm>
#include <istream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>

namespace
{

using Task = char;
using Time = size_t;
struct TaskDependency
{
  Task task_before;
  Task task_after;
};
struct TaskTime
{
  Time time;
  Task task;
};
bool operator<( const TaskTime tt1, const TaskTime tt2 )
{
  return std::tie( tt1.time, tt1.task ) < std::tie( tt2.time, tt2.task );
}

using Tasks             = std::set<Task>;
using TaskDependencyMap = std::map<Task, Tasks>;
using DependencyMap     = std::multimap<Task, Task>;
using TaskTimeSet       = std::set<TaskTime>;

using Seconds     = size_t;
using BusyWorkers = std::vector<Seconds>;

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( TaskDependency, task_before, task_after )

namespace
{
TaskDependency parse_dependency( const std::string& line )
{
  namespace x3      = boost::spirit::x3;
  const auto parser = "Step" > x3::char_ > "must" > "be" > "finished" > "before" > "step" > x3::char_ > "can" > "begin";

  TaskDependency task;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space | x3::punct, task );
  if ( !is_parsed )
  {
    throw std::runtime_error( "Failed to parse task input" );
  }

  return task;
}

Time calc_task_time( const Task task, const Time task_base_time )
{
  assert( isupper( task ) );
  const Time task_time = boost::numeric_cast<Time>( task - 'A' );
  return task_base_time + task_time + 1;
}

bool has_all_dependencies_done( const Task task, const TaskDependencyMap& dependency_map )
{
  auto iter = dependency_map.find( task );
  assert( iter != dependency_map.cend() );

  const auto& dependencies = iter->second;
  return dependencies.empty();
}

boost::optional<Task> try_get_next_ready_to_start_task( const Tasks& reachable_tasks, const TaskDependencyMap& dependency_map )
{
  auto task_iter =
      ranges::find_if( reachable_tasks, [&dependency_map]( const Task t ) { return has_all_dependencies_done( t, dependency_map ); } );

  if ( task_iter == reachable_tasks.cend() )
  {
    return {};
  }

  return *task_iter;
}

void complete_task( const Task done_task, Tasks& reachable_tasks, TaskDependencyMap& dependency_map )
{
  reachable_tasks.erase( done_task );

  for ( auto& [ task, dependencies ] : dependency_map )
  {
    const auto task_deleted = dependencies.erase( done_task );
    if ( task_deleted )
    {
      reachable_tasks.insert( task );
    }
  }
}

auto parse_input( std::istream& input )
{
  const auto dependencies = ranges::getlines( input ) | ranges::view::transform( &parse_dependency ) | ranges::to_vector;

  TaskDependencyMap dependency_map;
  for ( auto dependency : dependencies )
  {
    dependency_map[ dependency.task_before ];
    dependency_map[ dependency.task_after ].insert( dependency.task_before );
  }

  Tasks reachable_tasks = dependency_map |
                          ranges::view::filter( []( const TaskDependencyMap::value_type& kv ) { return kv.second.empty(); } ) |
                          ranges::view::keys | ranges::to<std::set>;

  AOC_RETURN_ANONYMOUS_STRUCT( dependency_map, reachable_tasks );
}

void sort_tasks( Tasks& reachable_tasks, TaskDependencyMap& dependency_map, std::string& result )
{
  if ( reachable_tasks.empty() )
  {
    return;
  }

  const Task next_ready_to_start_task = *try_get_next_ready_to_start_task( reachable_tasks, dependency_map );
  complete_task( next_ready_to_start_task, reachable_tasks, dependency_map );

  result.push_back( next_ready_to_start_task );
  return sort_tasks( reachable_tasks, dependency_map, result );
}

Time calc_total_time( Tasks& reachable_tasks,
                      TaskDependencyMap& dependency_map,
                      TaskTimeSet& tasks_in_work,
                      const size_t free_workers,
                      const Time task_base_time,
                      const Time total_time )
{
  if ( reachable_tasks.empty() && tasks_in_work.empty() )
  {
    return total_time;
  }

  if ( const auto next_ready_to_start_task = try_get_next_ready_to_start_task( reachable_tasks, dependency_map );
       free_workers > 0 && next_ready_to_start_task )
  {
    tasks_in_work.insert( { calc_task_time( *next_ready_to_start_task, task_base_time ), *next_ready_to_start_task } );
    reachable_tasks.erase( *next_ready_to_start_task );

    return calc_total_time( reachable_tasks, dependency_map, tasks_in_work, free_workers - 1, task_base_time, total_time );
  }

  assert( !tasks_in_work.empty() );

  const auto task_in_work = *tasks_in_work.begin();
  complete_task( task_in_work.task, reachable_tasks, dependency_map );

  tasks_in_work.erase( tasks_in_work.begin() );
  tasks_in_work = tasks_in_work | ranges::view::transform( [task_in_work]( TaskTime tt ) {
                    tt.time -= task_in_work.time;
                    return tt;
                  } ) |
                  ranges::to<std::set>;

  return calc_total_time(
      reachable_tasks, dependency_map, tasks_in_work, free_workers + 1, task_base_time, total_time + task_in_work.time );
}

size_t solve_2_impl( std::istream& input, const size_t workers, const size_t task_base_time )
{
  auto tasks = parse_input( input );

  TaskTimeSet tasks_in_work;
  const Time total_time = calc_total_time( tasks.reachable_tasks, tasks.dependency_map, tasks_in_work, workers, task_base_time, 0 );

  return total_time;
}

}  // namespace

namespace AoC_2018
{

namespace problem_07
{

std::string solve_1( std::istream& input )
{
  auto tasks = parse_input( input );

  std::string result;
  sort_tasks( tasks.reachable_tasks, tasks.dependency_map, result );
  return result;
}

size_t solve_2( std::istream& input )
{
  return solve_2_impl( input, 5, 60 );
}

AOC_REGISTER_PROBLEM( 2018_07, solve_1, solve_2 );

}  // namespace problem_07

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>
#  include <sstream>

static void impl_tests()
{
  std::istringstream input( R"(Step C must be finished before step A can begin.
                               Step C must be finished before step F can begin.
                               Step A must be finished before step B can begin.
                               Step A must be finished before step D can begin.
                               Step B must be finished before step E can begin.
                               Step D must be finished before step E can begin.
                               Step F must be finished before step E can begin.)" );

  assert( 15 == solve_2_impl( input, 2, 0 ) );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
