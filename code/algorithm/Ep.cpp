#include "Ep.h"
#include "../Constants.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "../ComputerPlayer.h"
	
/*top level class of the core algorithm, it provides interface to Player
  class, they are well modeled, the granity of this class is object Node
*/


Distribution Ep::descent(Node &n){
	if(n.isVisited()){
		Node* c = n.getChild();
		if(c!=NULL){
			c->sampleG();
			/*update message to c*/
			Ep::updateMessageFromParent(*c,n);
			/*continue to descent*/
			Distribution newMessage = descent(*c);
			/*update marginal of n*/
			Ep::updateMessageToParent(*c,n,newMessage);
		}
	}

	else{
		/*update message, include dividing roll out out of the distribution*/
		Node * parent  = n.getParent();
		Ep::updateMessageExceptRollOut(n,*parent);
		
		/*dealing with roll out*/
		/*assume this is returned by the roll out*/
	        
		int length = 0;
		int result = 0;
		char lastPlayer;
		std::vector<int> stored(0);
		Ep::doRollout(n,length,result,lastPlayer,stored);
		
		/*call function to calculated the approximate message*/
		Distribution rollOut = Ep::getMessageFromRollOut(n,length,result);	
		n.setGDis(n.getGDis()*rollOut);
		/*update V distribution =  delta + V*/
		Distribution v = Ep::calculateDelta(stored,lastPlayer);
		n.setVDis(v+n.getGDis());

		n.setVisited();
	}
	Distribution messageExceptP = n.getGDis()/n.getMessageFromParent();
	Ep::updateParentVDistribution(n);

	return Distribution(messageExceptP.getMean(),messageExceptP.getVar()+1);

}

/*change as different board and computer player implementation*/
void Ep::doRollout(Node &n,int &l,int &r,char &lastPlayer,std::vector<int> &stored){
	Board b = n.getBoard();
	char color = n.getColor();
	ComputerPlayer p(color);
	int num = 0;
	char result;
	while(1){
		 num = b.numberOfNextBoardStates(p.getColor());
		if(num==0){
			p.setColor(p.getColor()==WHITE ? BLACK : WHITE);
			break;
		}
		stored.push_back(num);
		result = p.randomPlay(b);
		if(result != CONTINUE)
			break;
		p.setColor(p.getColor()==WHITE ? BLACK : WHITE);
	}
	l = p.step; 
	lastPlayer =  p.getColor();
	r = result == 'X' ? 1 : -1;
}

Distribution Ep::calculateDelta(std::vector<int> &branch,char lastPlayer){
	if(branch.size()==0)
		return Distribution(0,0);

	Distribution delta(0,0);
	for(std::vector<int>::iterator it = branch.end()-1;it>=branch.begin();it--){
		int branchFactor = (*it);
		std::vector<Distribution> variables(0);
		for(int i=0;i<branchFactor;i++){
			variables.push_back(delta+Distribution(0,1));

		}
		if(lastPlayer == MAX){
			delta =  Distribution::getMaxOfIndependentSet(variables);
			lastPlayer = MIN;
		}
		else{
			delta =  Distribution::getMinOfIndependentSet(variables);
			lastPlayer = MAX;
		}
	}
	return delta;
}

void Ep::updateParentVDistribution(Node &n){
	Node *parent = n.getParent();
	if(parent==NULL)
		return;
	
	std::vector<Node*> variables(0);
	std::vector<Node*> children = parent->getChildren();
	for(std::vector<Node*>::iterator it=children.begin();it<children.end();it++){
		if((*it)->getVDis()==Distribution(0,0)){
			continue;
		}
		else{
			variables.push_back((*it));
		}
	}
	Distribution newV;
	if(parent->getColor()==MAX)
		newV = Distribution::getMaxOfCorrelatedSet(variables);
	else
		newV = Distribution::getMinOfCorrelatedSet(variables);
	
	parent->setVDis(newV);
}

Distribution Ep::getMessageFromRollOut(Node &boundary,const int length,const int result){
	Distribution prior = Distribution(boundary.getGDis().getMean(),boundary.getGDis().getVar()+length);
	double priorMean = prior.getMean();
	double priorVar = prior.getVar();

	double m,v,n,d,p,secondMoment;
	double meanSqaure = pow(priorMean,2);
	if(result>0){
		/*first moment*/
		n = Distribution(0,priorVar).pdf(priorMean);
		d = 1- prior.phi(0);
		/*seconde moment*/
		p = (1-(-priorMean*n)/d);
		
	}
	else{
		/*first moment*/
		n = -(Distribution(0,priorVar).pdf(priorMean));
		d =  prior.phi(0);
		/*second moment*/
		p  = (1-(priorMean*(-n)/d));

		}
	m = priorMean + priorVar*(n/d);
	secondMoment =  meanSqaure+ priorVar*p;
	/*variance*/
	v = secondMoment - pow(m,2);
	
	return Distribution(m,v+length);
}

void Ep::updateMessageFromParent(Node &child,Node &parent){
	child.setGDis(child.getGDis()/child.getMessageFromParent());
	Distribution messageExceptC = parent.getGDis()/child.getMessageToParent();
	Distribution messageIntegral = Distribution(messageExceptC.getMean(),messageExceptC.getVar()+1);
	child.setMessageFromParent(messageIntegral);
	child.setGDis(child.getGDis()*child.getMessageFromParent());
}

void Ep::updateMessageExceptRollOut(Node &child,Node &parent){
	Distribution newParent = parent.getGDis()/parent.getRollOut();
	child.setGDis(child.getGDis()/child.getMessageFromParent());
	Distribution messageExceptC = newParent/child.getMessageToParent();
	Distribution messageIntegral = Distribution(messageExceptC.getMean(),messageExceptC.getVar()+1);
	child.setGDis(child.getGDis()*messageIntegral);
	child.setMessageFromParent(messageIntegral);
}

void Ep::updateMessageToParent(Node &child,Node &parent,Distribution &message){
	
	parent.setGDis(parent.getGDis()/child.getMessageToParent()*message);
	child.setMessageToParent(message);

}

