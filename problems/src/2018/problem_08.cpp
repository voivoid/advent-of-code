#include "AoC/2018/problem_08.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"

#include <istream>
#include <vector>

namespace
{
auto read_node_data( std::istream& input )
{
  size_t child_nodes      = 0;
  size_t metadata_entries = 0;

  input >> child_nodes;
  input >> metadata_entries;

  AOC_RETURN_ANONYMOUS_STRUCT( child_nodes, metadata_entries );
}

size_t sum_metadata_entries( std::istream& input, size_t metadata_entries )
{
  size_t metadata_sum = 0;
  while ( metadata_entries )
  {
    size_t metadata = 0;
    input >> metadata;

    metadata_sum += metadata;
    --metadata_entries;
  }

  return metadata_sum;
}

size_t calc_metadata_sum_1( std::istream& input )
{
  auto node = read_node_data( input );

  size_t metadata_sum = 0;
  while ( node.child_nodes )
  {
    metadata_sum += calc_metadata_sum_1( input );
    --node.child_nodes;
  }

  metadata_sum += sum_metadata_entries( input, node.metadata_entries );

  return metadata_sum;
}

size_t calc_metadata_sum_2( std::istream& input )
{
  auto node = read_node_data( input );

  if ( node.child_nodes == 0 )
  {
    return sum_metadata_entries( input, node.metadata_entries );
  }

  std::vector<size_t> child_nodes_sums;
  child_nodes_sums.resize( node.child_nodes );
  for ( size_t i = 0; i < node.child_nodes; ++i )
  {
    child_nodes_sums[ i ] = calc_metadata_sum_2( input );
  }

  size_t metadata_sum = 0;
  while ( node.metadata_entries )
  {
    size_t metadata_index = 0;
    input >> metadata_index;
    if ( metadata_index != 0 && metadata_index <= child_nodes_sums.size() )
    {
      metadata_sum += child_nodes_sums[ metadata_index - 1 ];
    }

    --node.metadata_entries;
  }

  return metadata_sum;
}


}  // namespace

namespace AoC_2018
{

namespace problem_08
{

size_t solve_1( std::istream& input )
{
  const auto sum = calc_metadata_sum_1( input );
  return sum;
}

size_t solve_2( std::istream& input )
{
  const auto sum = calc_metadata_sum_2( input );
  return sum;
}

AOC_REGISTER_PROBLEM( 2018_08, solve_1, solve_2 );

}  // namespace problem_08

}  // namespace AoC_2018
