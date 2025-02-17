#include "MixedStrategyPSO.h"

void InitSwarmOfMSPSO(network _network,int funcID)
{	
	for(int i=0;i!=ParticleAmount;++i)
	{
		swarm[i].reset();
		swarm[i].particleID=i;
		swarm[i].neighborsID=_network[i].neighborsID;
		swarm[i].neighborsID.push_back(i);
	}
	for(int i=0;i!=ParticleAmount;++i)
	{
		for(int j=0;j!=Dimension;++j)
		{
			switch(funcID)
			{
			case 1:
				swarm[i].position[j]=getRandWithin_0_1()*(sphere.upper-sphere.lower)+sphere.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(sphere.upper-sphere.lower)+sphere.lower;
				break;
			case 2:
				swarm[i].position[j]=getRandWithin_0_1()*(rosenbrock.upper-rosenbrock.lower)+rosenbrock.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(rosenbrock.upper-rosenbrock.lower)+rosenbrock.lower;
				break;
			case 3:
				swarm[i].position[j]=getRandWithin_0_1()*(rastrigin.upper-rastrigin.lower)+rastrigin.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(rastrigin.upper-rastrigin.lower)+rastrigin.lower;
				break;
			case 4:
				swarm[i].position[j]=getRandWithin_0_1()*(qua.upper-qua.lower)+qua.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(qua.upper-qua.lower)+qua.lower;
				break;
			case 5:
				swarm[i].position[j]=getRandWithin_0_1()*(griwank.upper-griwank.lower)+griwank.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(griwank.upper-griwank.lower)+griwank.lower;
				break;
			case 6:
				swarm[i].position[j]=getRandWithin_0_1()*(ackley.upper-ackley.lower)+ackley.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(ackley.upper-ackley.lower)+ackley.lower;
				break;
			case 7:
				swarm[i].position[j]=getRandWithin_0_1()*(weierstrass.upper-weierstrass.lower)+weierstrass.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(weierstrass.upper-weierstrass.lower)+weierstrass.lower;
				break;
			case 8:
				swarm[i].position[j]=getRandWithin_0_1()*(noncontinuousRas.upper-noncontinuousRas.lower)+noncontinuousRas.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(noncontinuousRas.upper-noncontinuousRas.lower)+noncontinuousRas.lower;
				break;
			case 9:
				swarm[i].position[j]=getRandWithin_0_1()*(schwefel_P2_22.upper-schwefel_P2_22.lower)+schwefel_P2_22.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(schwefel_P2_22.upper-schwefel_P2_22.lower)+schwefel_P2_22.lower;
				break;
			case 10:
				swarm[i].position[j]=getRandWithin_0_1()*(noise_1.upper-noise_1.lower)+noise_1.lower;
				swarm[i].velocity[j]=getRandWithin_0_1()*(noise_1.upper-noise_1.lower)+noise_1.lower;
				break;
			}
		}
	}
	for(int i=0;i!=ParticleAmount;++i)
	{
		switch(funcID)
		{
		case 1:
			swarm[i].fitness=SphereCalc(swarm[i].position);break;
		case 2:
			swarm[i].fitness=RosenbrockCalc(swarm[i].position);break;
		case 3:
			swarm[i].fitness=RastriginCalc(swarm[i].position);break;
		case 4:
			swarm[i].fitness=QuaCalc(swarm[i].position);break;
		case 5:
			swarm[i].fitness=GriwenkCalc(swarm[i].position);break;
		case 6:
			swarm[i].fitness=AckleyCalc(swarm[i].position);break;
		case 7:
			swarm[i].fitness=WeierstrassCalc(swarm[i].position);break;
		case 8:
			swarm[i].fitness=NoncontinuousRasCalc(swarm[i].position);break;
		case 9:
			swarm[i].fitness=Schwefel_P2_22Calc(swarm[i].position);break;
		case 10:
			swarm[i].fitness=Noise_1Calc(swarm[i].position);break;

		}
		/**< set the initial value as the pbest and lbest */
		swarm[i].pbest=swarm[i].position;
		swarm[i].fpbest=swarm[i].fitness;
	}
	setLbest_FIPS();
	setGbest();
	//    cout<<"In initiation, fgbest="<<fgbest<<endl;
	//    system("pause");
}

void DivideSwarm(int FIPSAmount)//divide the swarm into two subswarm,one take traditional strategy, the other take FIPS
{
	for (int i=0;i!=ParticleAmount;++i)
	{
		swarm[i].isFIPS=0;
	}
	vector<int> FIPSID;
	for (int i=0;i!=FIPSAmount;++i)
	{
		while (1)
		{
			int temp=rand()%ParticleAmount;
			if (FIPSID.empty())
			{
				FIPSID.push_back(temp);
				break;
			}
			if (find(FIPSID.begin(),FIPSID.end(),temp)==FIPSID.end())
			{
				FIPSID.push_back(temp);
				break;
			}
		}
	}
	for (auto it=FIPSID.begin();it!=FIPSID.end();++it)
		swarm[*it].isFIPS=1;
}

void UpdateSwarmOfMSPSO(int funcID, int FIPSAmount)//the amount of particles that take the FIPS strategy
{
	DivideSwarm(FIPSAmount);
	for(int i=0;i!=ParticleAmount;++i)
	{
		if (swarm[i].isFIPS==0)
		{
			for(int j=0;j!=Dimension;++j)
			{
				swarm[i].velocity[j]=Chi*(swarm[i].velocity[j]+B*getRandWithin_0_1()*(swarm[i].pbest[j]-swarm[i].position[j])
					+B*getRandWithin_0_1()*(swarm[i].lbest[j]-swarm[i].position[j]));
				swarm[i].position[j]=swarm[i].position[j]+swarm[i].velocity[j];
			}
		}
		else
		{
			for(int j=0;j!=Dimension;++j)
			{
				swarm[i].velocity[j]=Chi*(swarm[i].velocity[j]+swarm[i].lbest_FIPS[j]);
				swarm[i].position[j]=swarm[i].position[j]+swarm[i].velocity[j];
			}
		}
	}
	for(int i=0;i!=ParticleAmount;++i)
	{
		switch(funcID)
		{
		case 1:
			swarm[i].fitness=SphereCalc(swarm[i].position);break;
		case 2:
			swarm[i].fitness=RosenbrockCalc(swarm[i].position);break;
		case 3:
			swarm[i].fitness=RastriginCalc(swarm[i].position);break;
		case 4:
			swarm[i].fitness=QuaCalc(swarm[i].position);break;
		case 5:
			swarm[i].fitness=GriwenkCalc(swarm[i].position);break;
		case 6:
			swarm[i].fitness=AckleyCalc(swarm[i].position);break;
		case 7:
			swarm[i].fitness=WeierstrassCalc(swarm[i].position);break;
		case 8:
			swarm[i].fitness=NoncontinuousRasCalc(swarm[i].position);break;
		case 9:
			swarm[i].fitness=Schwefel_P2_22Calc(swarm[i].position);break;
		case 10:
			swarm[i].fitness=Noise_1Calc(swarm[i].position);break;
		}
	}
}

performance MSPSO(network _network,int funcID,int FIPSAmount)
{
	InitSwarmOfMSPSO(_network,funcID);
	int iter=0;
	int hasConverged=0;
	performance resultOfThisRun;
	while(iter < MaxIteration)
	{
		UpdateSwarmOfMSPSO(funcID,FIPSAmount);
		setPbest();
		setLbest_FIPS();
		setGbest();
		switch(funcID)
		{
		case 1:
			if(fgbest<sphere.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 2:
			if(fgbest<rosenbrock.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 3:
			if(fgbest<rastrigin.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 4:
			if(fgbest<qua.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 5:
			if(fgbest<griwank.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 6:
			if(fgbest<ackley.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 7:
			if(fgbest<weierstrass.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 8:
			if(fgbest<noncontinuousRas.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 9:
			if(fgbest<schwefel_P2_22.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		case 10:
			if(fgbest<noise_1.goal && hasConverged==0)
			{
				hasConverged=1;
				resultOfThisRun.speed=iter;
			}
			break;
		}
		//        cout<<fgbest<<endl;
		++iter;
	}
	resultOfThisRun.solution=fgbest;
	return resultOfThisRun;
}

void runMSPSO()
{
	network inputNetwork;
	ofstream outputResults("resultsOfMSPSO.txt");
	srand((unsigned)time(0));
	rand();
	for(int k=1;k<=nodeNum/2;++k)
	{
		for (int FIAmount=0;FIAmount<=ParticleAmount;FIAmount+=5)
		{
			finalResult SphereResult;
			finalResult RosenbrockResult;
			finalResult RastriginResult;
			finalResult QuaResult;
			finalResult GriwankResult;
			finalResult AckleyResult;
			finalResult weierstrassResult;
			finalResult noncontinuousRasResult;
			finalResult schwefel_P2_22Result;
			finalResult noise_1Result;

			for(int funcID=1;funcID<=10;++funcID)
			{
				cout<<"k="<<k<<"\t"<<"FuncID="<<funcID<<endl;
				for(int nrepeat=0;nrepeat!=NetwRepeatNum;++nrepeat)
				{
					for (int i=0;i!=nodeNum;++i)
					{
						inputNetwork[i].reset();
					}
					inputNetwork=ringConstruct(k);
					//                cout<<"adjacMatrix generated successfully!"<<endl;
					//                for(int i=0;i!=nodeNum;++i)
					//                {
					//                    for(int j=0;j!=nodeNum;++j)
					//                        cout<<MatrixOfHierarchyNetwork[i][j]<<"\t";
					//                    cout<<endl;
					//                }
					//                system("pause");
					for(int arepeat=0;arepeat!=AlgoRepeatNum;++arepeat)
					{
						performance thisRun;
						thisRun=MSPSO(inputNetwork,funcID,FIAmount);
						switch(funcID)
						{
						case 1:
							SphereResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							SphereResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 2:
							RosenbrockResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							RosenbrockResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 3:
							RastriginResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							RastriginResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 4:
							QuaResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							QuaResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 5:
							GriwankResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							GriwankResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 6:
							AckleyResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							AckleyResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 7:
							weierstrassResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							weierstrassResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 8:
							noncontinuousRasResult.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							noncontinuousRasResult.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 9:
							schwefel_P2_22Result.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							schwefel_P2_22Result.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;
						case 10:
							noise_1Result.solutions[nrepeat*AlgoRepeatNum+arepeat]=thisRun.solution;
							noise_1Result.speeds[nrepeat*AlgoRepeatNum+arepeat]=thisRun.speed;
							break;

						}
					}
				}
			}
			SphereResult.finalSolution=SphereResult.calcuFinalSolution();
			SphereResult.finalSpeed=SphereResult.calcuFinalSpeed();
			SphereResult.succRate=SphereResult.calcuFinalSuccRate();

			RosenbrockResult.finalSolution=RosenbrockResult.calcuFinalSolution();
			RosenbrockResult.finalSpeed=RosenbrockResult.calcuFinalSpeed();
			RosenbrockResult.succRate=RosenbrockResult.calcuFinalSuccRate();

			RastriginResult.finalSolution=RastriginResult.calcuFinalSolution();
			RastriginResult.finalSpeed=RastriginResult.calcuFinalSpeed();
			RastriginResult.succRate=RastriginResult.calcuFinalSuccRate();

			QuaResult.finalSolution=QuaResult.calcuFinalSolution();
			QuaResult.finalSpeed=QuaResult.calcuFinalSpeed();
			QuaResult.succRate=QuaResult.calcuFinalSuccRate();

			GriwankResult.finalSolution=GriwankResult.calcuFinalSolution();
			GriwankResult.finalSpeed=GriwankResult.calcuFinalSpeed();
			GriwankResult.succRate=GriwankResult.calcuFinalSuccRate();

			AckleyResult.finalSolution=AckleyResult.calcuFinalSolution();
			AckleyResult.finalSpeed=AckleyResult.calcuFinalSpeed();
			AckleyResult.succRate=AckleyResult.calcuFinalSuccRate();

			weierstrassResult.finalSolution=weierstrassResult.calcuFinalSolution();
			weierstrassResult.finalSpeed=weierstrassResult.calcuFinalSpeed();
			weierstrassResult.succRate=weierstrassResult.calcuFinalSuccRate();

			noncontinuousRasResult.finalSolution=noncontinuousRasResult.calcuFinalSolution();
			noncontinuousRasResult.finalSpeed=noncontinuousRasResult.calcuFinalSpeed();
			noncontinuousRasResult.succRate=noncontinuousRasResult.calcuFinalSuccRate();

			schwefel_P2_22Result.finalSolution=schwefel_P2_22Result.calcuFinalSolution();
			schwefel_P2_22Result.finalSpeed=schwefel_P2_22Result.calcuFinalSpeed();
			schwefel_P2_22Result.succRate=schwefel_P2_22Result.calcuFinalSuccRate();

			noise_1Result.finalSolution=noise_1Result.calcuFinalSolution();
			noise_1Result.finalSpeed=noise_1Result.calcuFinalSpeed();
			noise_1Result.succRate=noise_1Result.calcuFinalSuccRate();


			outputResults<<k<<"\t"<<FIAmount
				<<"\t"<<SphereResult.finalSolution<<"\t"<<SphereResult.finalSpeed<<"\t"<<SphereResult.succRate
				<<"\t"<<RosenbrockResult.finalSolution<<"\t"<<RosenbrockResult.finalSpeed<<"\t"<<RosenbrockResult.succRate
				<<"\t"<<RastriginResult.finalSolution<<"\t"<<RastriginResult.finalSpeed<<"\t"<<RastriginResult.succRate
				<<"\t"<<QuaResult.finalSolution<<"\t"<<QuaResult.finalSpeed<<"\t"<<QuaResult.succRate
				<<"\t"<<GriwankResult.finalSolution<<"\t"<<GriwankResult.finalSpeed<<"\t"<<GriwankResult.succRate
				<<"\t"<<AckleyResult.finalSolution<<"\t"<<AckleyResult.finalSpeed<<"\t"<<AckleyResult.succRate
				<<"\t"<<weierstrassResult.finalSolution<<"\t"<<weierstrassResult.finalSpeed<<"\t"<<weierstrassResult.succRate
				<<"\t"<<noncontinuousRasResult.finalSolution<<"\t"<<noncontinuousRasResult.finalSpeed<<"\t"<<noncontinuousRasResult.succRate
				<<"\t"<<schwefel_P2_22Result.finalSolution<<"\t"<<schwefel_P2_22Result.finalSpeed<<"\t"<<schwefel_P2_22Result.succRate
				<<"\t"<<noise_1Result.finalSolution<<"\t"<<noise_1Result.finalSpeed<<"\t"<<noise_1Result.succRate
				<<endl;
		}
	}
	outputResults.close();
}