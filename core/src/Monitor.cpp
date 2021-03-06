#include "Monitor.h"
#include "analogprop.h"

Monitor::Monitor(Property* pt){
	property = pt;
	cout << "Monitor has been initialized" << endl ;
}

Monitor::~Monitor(){}

void Monitor::eval(node* v){
	cout << "Checking the monitor execution" << endl;
	//step 1: initialize a new copy constructor for the properties
	Property* p = new Property(property, v);
	AnalogProperty* ap = (AnalogProperty*)(p->argument);

	propertyInstance.push_back(p);
	cout << "Evaluation of the property is " << p->eval() << endl;

	//step 3: is there any violation?
	//Property* propAtInitialState = propertyInstance[0]->eval();
	//cout << "Property result : " << 
}

/*

#include <math.h>


Monitor::~Monitor(){
	
	for(unsigned int i=0;i<arguments.size(); i++){
		delete arguments[i];
	}
}

void Monitor::push_back(Monitor* monitor){
	arguments.push_back( new MonitorArgument(monitor) ) ;
}

void Monitor::push_back(double data){
	arguments.push_back( new MonitorArgument(data) ) ;
}

void Monitor::push_back(int data){
	arguments.push_back( new MonitorArgument(data));
}


double Monitor::getArgumentValue(int i, node* q_new){
	switch(arguments[i]->getType()){
	case MONITOR_ARGUMENT_DYNAMIC_TYPE:
		return arguments[i]->getMonitor()->verify(q_new) ; //This line may contains some bug ...
		break;

	case MONITOR_ARGUMENT_STATIC_TYPE:
		return arguments[i]->getValue();

	case MONITOR_ARGUMENT_SIGNAL:
		return q_new->get( arguments[i]->getIndex() ) ;
	}
}

//This methid is used to get the value for logical operators
bool Monitor::analyze(int i, node* q_new){
	if(arguments[i]->getType() != MONITOR_ARGUMENT_DYNAMIC_TYPE) throw 0;

//	arguments[i]->getMonitor()->update(q, )

}

void Monitor::update(node* q){
	if( status[q]==DONTKNOW ){
		double result = verify(q);
		//i

	}
}

//This method updates track, trend and status of each node in the monitor
void Monitor::update(node* q, bool value){
	if( status[q] == DONTKNOW ){
		status[q] = (value)? SATISFIED : VIOLATED ;
		if(q->isRoot()){
			track[q]=(value? 1: 0);
		}else{
			track[q] = track[q->getParent()] + (value? 1: 0);
		}
		if(q->isRoot()){
			trend[q] = (status[q]==SATISFIED)? true : false ;
		}else if(status[q]==SATISFIED && trend[ q->getParent() ] == true){
			trend[q]=true;
		}else{
			trend[q]=false;
		}
		cout << "Monitor value[" << type << "]=" << value << endl ;
	}
}
//This is the main function, this function will be called after adding each new node to the RRT
double Monitor::verify(node* q_new){
	//node* q = &(rrt->getNode(q_new));
	node* nod ;
	int step=0;
	double time=0;
	double tmp=0, tmp2=0, tmp_max=0, tmp_min=0;
	switch( type ){
	case CONSTANT:
		return getArgumentValue(0, q_new);

	case ANALOG_BINARY_ADD:
		return getArgumentValue(0, q_new) + getArgumentValue(1, q_new) ;

	case ANALOG_BINARY_SUB: 
		tmp= getArgumentValue(0, q_new) - getArgumentValue(1, q_new) ;
		cout << "Monitor value=" << tmp << endl ;
		return tmp;

	case ANALOG_BINARY_MUL: 
		return getArgumentValue(0, q_new) * getArgumentValue(1, q_new) ;

	case ANALOG_BINARY_DIV: 
		if( abs( getArgumentValue(1, q_new) ) < epsilon ){
			throw MONITOR_EXCEPTION_DIVISION_BY_ZERO ;
		}else{
			return getArgumentValue(0, q_new) / getArgumentValue(1, q_new) ;
		}
	case ANALOG_SHIFT: 
		//traverse the RRT upward to the root until we reach the node with the shifted time
		time = getArgumentValue(0, q_new) ;
		if(time<=0) throw MONITOR_EXCEPTION ;
		step = time/rrt->getdt();
		nod = q_new->getParent();
		while(step>0 && !(nod->isRoot())){
			nod = nod->getParent() ;
			step--;
			//				cout << "parent=" << parent << "\t\t\t" << step <<  endl ;
		}
		break;
	case ANALOG_NORM_L2:
		//This is the L2 norm for the signals or values; it is defined as \sqrt(\sigma_k=1^n |x_k|^2)
		tmp=0; tmp2=0;
		for(int i=0;i<arguments.size();i++){
			tmp2 = (getArgumentValue(i, q_new));
			tmp += tmp2*tmp2 ;
		}
		tmp = sqrt(tmp); 
		return tmp;
		break;


	case ANALOG_MIN_SIBLING:
		//list siblings
		tmp = numeric_limits<double>::max();
		for(int i=0;i<q_new->getParent()->getSize(); i++){
			tmp2 = getArgumentValue( 0, q_new->getParent()->getChild(i) ) ;
			if(tmp2<tmp) tmp=tmp2 ;
		}
		return tmp ;
		break;

	case ANALOG_MAX_SIBLING:
		tmp = -9999999;
		for(int i=0;i<q_new->getParent()->getSize(); i++){
			tmp2 = getArgumentValue( 0, q_new->getParent()->getChild(i) ) ;
			if(tmp2>tmp) tmp=tmp2 ;
		}
		return tmp ;
		break;

	case ANALOG_DIFF_SIBLING:
		tmp_max = -9999999;
		tmp_min = 9999999;
		for(int i=0;i<q_new->getParent()->getSize(); i++){
			tmp2 = getArgumentValue( 0, q_new->getParent()->getChild(i));
			if(tmp2>tmp_max) tmp_max=tmp2 ;
			if(tmp2<tmp_min) tmp_min=tmp2 ;
		}

		cout << "DIFF+========>>>>> " << tmp_max << " " << tmp_min << " " << tmp_max-tmp_min  << endl ;
		return tmp_max-tmp_min ;
		break;


	case LOGIC_LESS_THAN:
		tmp= getArgumentValue(0, q_new) < getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;

		break;
	case LOGIC_GREATER_THAN:
		tmp= getArgumentValue(0, q_new) > getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;

		break;
	case LOGIC_EQUAL:
		tmp= (getArgumentValue(0, q_new) == getArgumentValue(1, q_new) );
		update(q_new, tmp);
		return tmp;

		break;
	case LOGIC_LESS_THAN_OR_EQUAL:
		tmp= getArgumentValue(0, q_new) <= getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;

		break;


	case LOGIC_GREATER_THAN_OR_EQUAL:
		tmp= getArgumentValue(0, q_new) >= getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;

		break;


	case LOGIC_AND:
		tmp =  getArgumentValue(0, q_new) && getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;
		break;

	case LOGIC_OR:
		if(arguments[0]->getMonitor()->status[q_new] == SATISFIED || arguments[0]->getMonitor()->status[q_new] == SATISFIED){

		}else{

		}
		tmp =  getArgumentValue(0, q_new) || getArgumentValue(1, q_new) ;
		update(q_new, tmp);
		return tmp;
		break;

	case LOGIC_EVENTUALLY:
		tmp = getArgumentValue(0, q_new);
		arguments[0]->getMonitor()->update(q_new, tmp);
		if(arguments[0]->getMonitor()->status[q_new] == SATISFIED 
			|| arguments[0]->getMonitor()->status[q_new->getParent()]==SATISFIED ){
				status[q_new]=SATISFIED;
		}else{
			status[q_new]=VIOLATED;
		}

		break;
	default: break;
	}
}


Monitor* createMonitor_1(RRT* rrt){
	Monitor* m_const5mv = new Monitor(rrt, CONSTANT);
	m_const5mv->push_back(0.005);

	return m_const5mv;
}

//PLL locking property
// pll is locked when (v(pll_e)-v(pll_eb)) is settled to near zero for some time. 
Monitor* pllLockingPropertyMonitor(RRT* rrt){
	//(v(pll_e)-v(pll_eb))
	Monitor* m0 = new Monitor(rrt, ANALOG_BINARY_SUB );		
	m0->push_back(pll_e);
	m0->push_back(pll_eb);		

	//Norm( v(pll_e)-v(pll_eb) )
	Monitor* m1 = new Monitor( rrt, ANALOG_NORM_L2 ) ;
	m1->push_back(m0);

	//0.005mv
	Monitor* m_const5mv = new Monitor(rrt, CONSTANT);
	m_const5mv->push_back(0.005);
	
	Monitor* m3 = new Monitor(rrt, ANALOG_DIFF_SIBLING);
	m3->push_back(m1);
	
	//Norm( v(pll_e)-v(pll_eb) )<0.005mv
	Monitor* m2 = new Monitor(rrt, LOGIC_LESS_THAN_OR_EQUAL ) ;
	m2->push_back(m3);
	m2->push_back(m_const5mv);


	//Monitor* m2 = new Monitor(rrt, ANALOG_SHIFT ) ;			// m2 = x[t-(10e-7)]
	//m2->push_back(10e-7);

	//Monitor* m3 = new Monitor(rrt, ANALOG_BINARY_MUL) ;		// m3 = (x_2 + 0.5) * x[t-(10e-7)]
	//m3->push_back(m1); 
	//m3->push_back(m2);
	
	return m2 ;
}
*/