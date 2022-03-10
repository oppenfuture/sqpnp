//
// main.cpp
//
// Manolis Lourakis (lourakis **at** ics forth gr), September 2020
// 
// Example demo program for SQPnP with data points loaded from plain 2D arrays
//

#include <vector>
#include <iostream>
#include <chrono>

#include <types.h>
#include <sqpnp.h>


int main()
{
  const int n=4;

  double pts3[n][3]={
  {0.49000000953674316, 0.5099999904632568, 0.0},
  {0.5099999904632568, 0.5099999904632568, 0.0},
  {0.5099999904632568, 0.49000000953674316, 0.0},
  {0.49000000953674316, 0.49000000953674316, 0.0},
  };

  double pts2[n][2]={
#if 0
  // no noise
  {0.139024436737141, -0.00108631784422283},
  {0.149897105048989, 0.270584578309815},
  {-0.118448642309468, -0.0844116551810971},
  {0.0917181969674735, 0.0435196877212059},
  {0.100243308685939, -0.178506520365217},
  {-0.296312157121094, 0.220675975198136},
  {-0.331509880499455, -0.213091587841007},
#else
  // noisy
  {0.14278465093568316, 0.2800731054068116},
  {0.1414839440117931, 0.23769289948219477},
  {0.10021482310385836, 0.23974338394390857},
  {0.10107273480856897, 0.2818524802209963},
#endif
  };

  auto start = std::chrono::high_resolution_clock::now();

  std::vector<sqpnp::_Point> _3dpoints(n);
  std::vector<sqpnp::_Projection> _projections(n);

  for(int i=0; i<n; ++i){
    const double *p3=pts3[i];
    const double *p2=pts2[i];

    _3dpoints[i]=sqpnp::_Point(p3[0], p3[1], p3[2]);
    _projections[i]=sqpnp::_Projection(p2[0], p2[1]);
  }

  // equal weights for all points
  sqpnp::PnPSolver solver(_3dpoints, _projections, std::vector<double>(n, 1.0));

  auto stop = std::chrono::high_resolution_clock::now();

  if(solver.IsValid()){
    solver.Solve();
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "SQPnP found " << solver.NumberOfSolutions() << " solution(s)"<< std::endl;
    for (int i = 0; i < solver.NumberOfSolutions(); i++)
    {
      std::cout << "\nSolution " << i << ":\n";
      std::cout << *solver.SolutionPtr(i) << std::endl;
      std::cout << " Average squared projection error : " << solver.AverageSquaredProjectionErrors().at(i) << std::endl;
    }
  }
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cout << "Time taken by SQPnP: " << duration.count() << " microseconds" << std::endl << std::endl;

  return 0;
}
