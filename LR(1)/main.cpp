#include<iostream>
#include<map>
#include<queue>
#include<stack>
#include<windows.h>
using namespace std;

map<string,string>* initMap();
queue<string> Input();
int match(stack<string>&s_base,string input,map<string,string>*m,pair<string,int>*p);
void Calculate(map<string,string>*m,queue<string> q_input,pair<string,int>*p);
void displayMap(map<string,string>* m,int n);
void displayStack(stack<string>s);
void displayQueue(queue<string>q);

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
queue<string> Input(){
    queue<string>q_input;
    string input;
    getline(cin,input);
    for(int i=0;i<input.length();i++){
        string temp="";
        if(input[i]==' ')continue;
        else if(input[i]>='a'&&input[i]<='z'){
            while(input[i]>='a'&&input[i]<='z'){
                temp+=input[i];
                i++;
            }
            i--;
        }
        else{
            temp+=input[i];
        }
        q_input.push(temp);
        //cout<<temp<<endl;
    }
    return q_input;
}
int match(stack<string>&s_base,string input,map<string,string>*m,pair<string,int>*p){
    //0 acc 1 push -1 error
    string t=s_base.top();
    int state=-1;
    if(t.length()==1){
            state=t[0]-'0';
        }
        else{
            state=(t[0]-'0')*10+(t[1]-'0');
        }
    string go=m[state][input];//s7
    if(go=="acc"){
        cout<<"acc"<<endl;
        return 0;
    }
    cout<<"go"<<go<<"\t";
    if(go[0]=='s'){
        string subGo=go.substr(1,go.length()-1);
        s_base.push(input);
        s_base.push(subGo);
        cout<<"yijin   "<<input<<endl;
        return 1;
    }
    else if(go[0]=='r'){
        int step=go[1]-'0';
        cout<<p[step].first<<"   guiyue"<<endl;
        string unFin=p[step].first.substr(0,1);
        step=(p[step].second)*2;
        for(int i=0;i<step;i++){
            s_base.pop();
        }
        string nowTop=s_base.top();
        int nowstate=0;
        if(nowTop.length()==1){
            nowstate=nowTop[0]-'0';
        }
        else{
            nowstate=(nowTop[0]-'0')*10+(nowTop[1]-'0');
        }
        s_base.push(unFin);
        s_base.push(m[nowstate][unFin]);
        return 2;
    }
    else{
        return -1;
    }
}
void Calculate(map<string,string>*m,queue<string> q_input,pair<string,int>*p){
    stack<string>s_base;
    s_base.push("0");
    while(!q_input.empty()){
        displayStack(s_base);
        displayQueue(q_input);
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
void displayStack(stack<string>s){
    stack<string>temp;
    while(!s.empty()){
        temp.push(s.top());
        s.pop();
    }
    while(!temp.empty()){
        s.push(temp.top());
        cout<<s.top();
        temp.pop();
    }
    cout<<"\t";
}
void displayQueue(queue<string>q){
    queue<string>temp;
    while(!q.empty()){
        temp.push(q.front());
        cout<<q.front();
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
    queue<string>q_input=Input();
    Calculate(m,q_input,p);
    system("pause");
    return 0;
}
/*
id*id+id$
id*+id+(id+id)#
*/
