#include "../include/adjacency_matrix.cpp"

void println(std::string msg)
{
  std::cout << msg << std::endl;
  return ;
}

int main(int argc, char * argv[])
{
  cnsd::AdjacencyMatrix<double> u_am(3, cnsd::CON_UNDIRECTED);
  cnsd::AdjacencyMatrix<int> d_am(5);

  cnsd::AdjacencyMatrix<double> input_u_am;
  cnsd::AdjacencyMatrix<int> input_d_am;

  cnsd::AdjacencyMatrix<double> thresh_u_am;
  cnsd::AdjacencyMatrix<int> thresh_d_am;

  cnsd::debug::enable_log(true);

  println("=> Empty Undirected Matrix");
  u_am.print();

  println("=> Edges Connected");
  u_am.setWeight(1,2, 1.5);
  u_am.setWeight(0,1, 3.2);
  u_am.print();

  println("=> Empty Undirected Matrix");
  d_am.print();

  println("=> Edges Connected");
  d_am.setWeight(1,2, 8);
  d_am.setWeight(4,4, 999);
  d_am.setWeight(0,1, 3);
  d_am.print();

  println("=> Output to files");
  u_am.toFile("files/u_am.txt");
  d_am.toFile("files/d_am.txt");


  println("=> Input from files");
  input_u_am.fromFile("files/u_am.txt");
  input_d_am.fromFile("files/d_am.txt");

  println("=> Undirected");
  input_u_am.print();
  println("=> Directed");
  input_d_am.print();

  println("=> Threshold Graphs - Undirected");
  u_am.cut(thresh_u_am, 2, 10, -1.0);
  thresh_u_am.print();

  println("=> Threshold Graphs - Directed");
  d_am.cut(thresh_d_am, 1, 10, -1);
  thresh_d_am.print();
  return 0;
}
