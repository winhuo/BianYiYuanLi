#include<iostream>
#include<stack>
#include<map>
#include<queue>
using namespace std;
class Stack_content{
public:
    string sym;
    int val;
};
map<string,string>* initMap();
pair<string,int>* initPair();
queue<Stack_content> Input();
void displayMap(map<string,string>* m,int n);
void displayStackString(stack<Stack_content>s);
void displayStackVal(stack<Stack_content>s);
void displayQueue(queue<Stack_content>q);

map<string,string>* initMap(){
    map<string,string>* m= new map<string,string>[15];
    m[0]["id"]="s5";m[0]["+"]="error";m[0]["*"]="error";m[0]["("]="s4";m[0][")"]="error";
    m[0]["$"]="error";m[0]["E"]="1";m[0]["T"]="2";m[0]["F"]="3";

    m[1]["id"]="error";m[1]["+"]="s6";m[1]["*"]="error";m[1]["("]="error";m[1][")"]="error";
    m[1]["$"]="acc";m[1]["E"]="error";m[1]["T"]="error";m[1]["F"]="error";

    m[2]["id"]="error";m[2]["+"]="r2";m[2]["*"]="s7";m[2]["("]="error";m[2][")"]="r2";
    m[2]["$"]="r2";m[2]["E"]="error";m[2]["T"]="error";m[2]["F"]="error";

    m[3]["id"]="error";m[3]["+"]="r4";m[3]["*"]="r4";m[3]["("]="error";m[3][")"]="r4";
    m[3]["$"]="r4";m[3]["E"]="error";m[3]["T"]="error";m[3]["F"]="error";

    m[4]["id"]="s5";m[4]["+"]="error";m[4]["*"]="error";m[4]["("]="s4";m[4][")"]="error";
    m[4]["$"]="error";m[4]["E"]="8";m[4]["T"]="2";m[4]["F"]="3";

    m[5]["id"]="error";m[5]["+"]="r6";m[5]["*"]="r6";m[5]["("]="error";m[5][")"]="r6";
    m[5]["$"]="r6";m[5]["E"]="error";m[5]["T"]="error";m[5]["F"]="error";

    m[6]["id"]="s5";m[6]["+"]="error";m[6]["*"]="error";m[6]["("]="s4";m[6][")"]="error";
    m[6]["$"]="error";m[6]["E"]="error";m[6]["T"]="9";m[6]["F"]="3";

    m[7]["id"]="s5";m[7]["+"]="error";m[7]["*"]="error";m[7]["("]="s4";m[7][")"]="error";
    m[7]["$"]="error";m[7]["E"]="error";m[7]["T"]="error";m[7]["F"]="10";

    m[8]["id"]="error";m[8]["+"]="s6";m[8]["*"]="error";m[8]["("]="error";m[8][")"]="s11";
    m[8]["$"]="error";m[8]["E"]="error";m[8]["T"]="error";m[8]["F"]="error";

    m[9]["id"]="error";m[9]["+"]="r1";m[9]["*"]="s7";m[9]["("]="error";m[9][")"]="r1";
    m[9]["$"]="r1";m[9]["E"]="error";m[9]["T"]="error";m[9]["F"]="error";

    m[10]["id"]="error";m[10]["+"]="r3";m[10]["*"]="r3";m[10]["("]="error";m[10][")"]="r3";
    m[10]["$"]="r3";m[10]["E"]="error";m[10]["T"]="error";m[10]["F"]="error";

    m[11]["id"]="error";m[11]["+"]="r5";m[11]["*"]="r5";m[11]["("]="error";m[11][")"]="r5";
    m[11]["$"]="r5";m[11]["E"]="error";m[11]["T"]="error";m[11]["F"]="error";
    //displayMap(m,12);
    return m;
}
pair<string,int>* initPair(){
    pair<string,int>*p=new pair<string,int>[10];
    p[1].first="E->E+T";p[1].second=3;
    p[2].first="E->T";p[2].second=1;
    p[3].first="T->T*F";p[3].second=3;
    p[4].first="T->F";p[4].second=1;
    p[5].first="F->(E)";p[5].second=3;
    p[6].first="F->id";p[6].second=1;
    return p;
}
queue<Stack_content> Input(){
    queue<Stack_content>q_input;
    string input;
    Stack_content temp;
    getline(cin,input);
    for(int i=0;i<input.length();i++){
        temp.sym="";
        temp.val=0;
        if(input[i]==' ')continue;
        else if(input[i]>='0'&&input[i]<='9'){
            temp.val=0;temp.sym="id";
            while(input[i]>='0'&&input[i]<='9'){
                temp.val*=10;
                temp.val+=input[i]-48;
                i++;
            }
            i--;
        }
        else{
            temp.sym+=input[i];
            temp.val=0;
        }
        q_input.push(temp);
    }
    temp.sym="$";
    temp.val=0;
    q_input.push(temp);
    return q_input;
}
int rule(stack<Stack_content>&s_base,int step){
    int a[50],i=0;
    for(int j=0;j<50;j++)a[i]=0;
    stack<Stack_content>s_temp;
    while(!s_base.empty()){
        s_temp.push(s_base.top());
        a[i]=s_temp.top().val;
        s_base.pop();
        i++;
    }
    while(!s_temp.empty()){
        s_base.push(s_temp.top());
        s_temp.pop();
    }
    switch(step){
        case 1:
            //top-1 + top-5
            return a[1]+a[5];
        case 2:
            //top-1
            return a[1];
        case 3:
            //top-1 * top-5
            return a[1]*a[5];
        case 4:
            //top-1
            return a[1];
        case 5:
             //top-3
             return a[3];
        case 6:
             //top-1
             return a[1];
        default:
            return -1000;
    }
}
int match(stack<Stack_content>&s_base,Stack_content input,map<string,string>*m,pair<string,int>*p){
    //0 acc 1 push -1 error
    Stack_content tempSC;
    string t=s_base.top().sym;
    int state=-1;
    if(t.length()==1){
            state=t[0]-'0';
        }
    else{
        state=(t[0]-'0')*10+(t[1]-'0');
    }
    string go=m[state][input.sym];//s7
    if(go=="acc"){
        s_base.pop();
        cout<<endl;
        cout<<"匹配结束，结果是："<<s_base.top().val<<endl;
        return 0;
    }
    cout<<"执行"<<go<<"\t";
    if(go[0]=='s'){
        string subGo=go.substr(1,go.length()-1);
        tempSC.sym=subGo;
        tempSC.val=0;
        s_base.push(input);
        s_base.push(tempSC);
        cout<<"移进"<<input.sym<<"完成"<<endl;
        return 1;
    }
    //归约
    else if(go[0]=='r'){
        int step=go[1]-'0';
        cout<<"按"<<p[step].first<<"归约"<<endl;
        string unFin=p[step].first.substr(0,1);
        int unFinVal=rule(s_base,step);
        Stack_content unF;
        unF.sym=unFin;
        unF.val=unFinVal;
        if(unFinVal==-1000){
            cout<<"error"<<endl;
            return -1;
        }
        step=(p[step].second)*2;
        for(int i=0;i<step;i++){
            s_base.pop();
        }
        string nowTop=s_base.top().sym;
        int nowstate=0;
        if(nowTop.length()==1){
            nowstate=nowTop[0]-'0';
        }
        else{
            nowstate=(nowTop[0]-'0')*10+(nowTop[1]-'0');
        }
        s_base.push(unF);
        unF.sym=m[nowstate][unFin];
        unF.val=0;
        s_base.push(unF);
        return 2;
    }
    else{
        return -1;
    }
}
void Calculate(map<string,string>*m,queue<Stack_content> q_input,pair<string,int>*p){
    stack<Stack_content>s_base;
    Stack_content tempSc;
    tempSc.sym="0";
    tempSc.val=0;
    s_base.push(tempSc);
    while(!q_input.empty()){
        displayStackString(s_base);
        displayQueue(q_input);
        displayStackVal(s_base);
        int state=match(s_base,q_input.front(),m,p);
        if(state==0)break;
        else if(state==1){
            q_input.pop();
        }
        else if(state==2){
            continue;
        }
        else {
            cout<<"error"<<endl;
            break;
        }
    }
}
void displayMap(map<string,string>* m,int n){
    map<string,string>::iterator it;
    for(int i=0;i<n;i++){
        if(i==0){
                cout<<"\t";
            for(it=m[0].begin();it!=m[0].end();it++){
                cout<<it->first<<"\t";
            }
        }
        cout<<endl;
        cout<<i<<"\t";
        for(it=m[i].begin();it!=m[i].end();it++){
            cout<<it->second<<"\t";
        }
        cout<<endl;
    }
}
void displayStackString(stack<Stack_content>s){
    stack<Stack_content>temp;
    while(!s.empty()){
        temp.push(s.top());
        s.pop();
    }
    while(!temp.empty()){
        s.push(temp.top());
        cout<<s.top().sym;
        temp.pop();
    }
    cout<<"\t";
}
void displayStackVal(stack<Stack_content>s){
    stack<Stack_content>temp;
    while(!s.empty()){
        temp.push(s.top());
        s.pop();
    }
    while(!temp.empty()){
        s.push(temp.top());
        cout<<s.top().val;
        temp.pop();
    }
    cout<<"\t";
}
void displayQueue(queue<Stack_content>q){
    queue<Stack_content>temp;
    while(!q.empty()){
        temp.push(q.front());
        cout<<q.front().sym;
        q.pop();
    }
    cout<<endl;
    while(!temp.empty()){
        q.push(temp.front());
        temp.pop();
    }
}
int main(){
    map<string,string>* m=initMap();
    pair<string,int>*p=initPair();
    queue<Stack_content>q=Input();
    displayQueue(q);
    Calculate(m,q,p);
}
/*
5*(3+2)
5*(3
*/
