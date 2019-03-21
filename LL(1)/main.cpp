#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM 20

char nonTerminal[MAX_NUM][256];
char terminal[MAX_NUM];
int num = 0;
int numTem = 0;

struct NoneTerminal {
	char nonTerm[256];
	int index;
	NoneTerminal*next;
};

struct SpreadString {
	NoneTerminal*nonTerm;
	bool emptyAble;
	SpreadString*next;
};

SpreadString*spread[MAX_NUM];

struct FirstChar {
	char firstChar;
	int spreadIndex;	//由哪个spread式子得出
	FirstChar*next;
};

struct FollowChar {
	char followChar;
	FollowChar*next;
};

FirstChar*first[MAX_NUM];
bool Efirst[MAX_NUM];
int Eindex[MAX_NUM];

FollowChar*follow[MAX_NUM];
bool Efollow[MAX_NUM];

bool finded[MAX_NUM];

struct CheckList {
	int speIndex;	//spread索引
};

CheckList cl[MAX_NUM][MAX_NUM];

void deal(char*s) {
	int index = 0;
	char name[256];
	while (*s != '=') {
		if (*s == ' ') {
			s++;
			continue;
		}
		name[index++] = *s;
		s++;
	}
	s++;
	name[index] = 0;
	strcpy(nonTerminal[num], name);
	SpreadString*st = NULL;
	index = 0;
	while ((*s != 0) && (*s != 10)) {
		bool check2 = false;
		bool check3 = false;
		SpreadString*t = new SpreadString;
		t->nonTerm = NULL;
		NoneTerminal*nt = NULL;
		while (true) {
			if (*s == ' ') {	//读完空格
				s++;
				continue;
			}
			if (*s == '&') {
				check3 = true;	//读到空的标志
				break;
			}
			if ((*s >= 'A'&&*s <= 'Z')||check2) {	//当读取到一个非终结符，则往下读'以将整个非终结符的名字确定
				if ((*s != '\'') && check2) {	//check2用于表示当前处于读'的状态
					name[index] = 0;	//如果发现下一个不是'且处于读'的状态，则说明一个终结符阅读完毕
					NoneTerminal*tnt = new NoneTerminal;
					strcpy(tnt->nonTerm, name);
					tnt->index = -1;
					tnt->next = NULL;
					if (!t->nonTerm) {
						t->nonTerm = tnt;
						nt = tnt;
					}
					else {
						nt->next = tnt;
						nt = nt->next;
					}
					index = 0;
					check2 = false;
					if (!((*s != '|') && (*s != 0) && (*s != 10)))
						break;
					continue;
				}
				check2 = true;	//读到终结符结束置为True
				name[index++] = *s;
			}
			else {	//如果读到了终结符
				if (!((*s != '|') && (*s != 0) && (*s != 10)))
					break;
				name[0] = *s;
				name[1] = 0;
				NoneTerminal*tnt = new NoneTerminal;
				strcpy(tnt->nonTerm, name);
				tnt->index = -1;
				tnt->next = NULL;
				if (!t->nonTerm) {
					t->nonTerm = tnt;
					nt = tnt;
				}
				else {
					nt->next = tnt;
					nt = nt->next;
				}
				//break;	//是否读到终结符就结束这一段spread的读取
			}
			if (!((*s != '|') && (*s != 0) && (*s != 10)))
				break;
			s++;
		}
		while ((*s != '|') && (*s != 0) && (*s != 10)) {
			s++;
		}
		s++;
		t->next = st;
		t->emptyAble = check3;
		st = t;
	}
	spread[num++] = st;
}

void findIndex() {
	for (int i = 0; i < num; i++) {
		SpreadString*t = spread[i];
		while (t) {
			NoneTerminal*nt = t->nonTerm;
			while (nt) {
				for (int j = 0; j < num; j++) {
					if (!strcmp(nt->nonTerm, nonTerminal[j])) {
						nt->index = j;
						break;
					}
				}
				nt = nt->next;
			}
			t = t->next;
		}
	}
}

void DFS(int index) {
	Eindex[index] = -1;
	finded[index] = true;
	first[index] = NULL;
	SpreadString*t = spread[index];
	int count = 0;
	while (t) {
		if (t->emptyAble) {
			Efirst[index] = true;
			Eindex[index] = count;
		}
		else {
			NoneTerminal*nt = t->nonTerm;
			while (nt) {
				if (nt->index == -1) {
					FirstChar*ft = new FirstChar;
					ft->firstChar = nt->nonTerm[0];
					ft->spreadIndex = count;
					ft->next = first[index];
					first[index] = ft;
					break;
				}
				else {
					if (!finded[nt->index])
						DFS(nt->index);
					FirstChar*ft = first[nt->index];
					while (ft) {
						FirstChar*ftt = new FirstChar;
						ftt->firstChar = ft->firstChar;
						ftt->next = first[index];
						ftt->spreadIndex = count;
						first[index] = ftt;
						ft = ft->next;
					}
					if (!Efirst[nt->index])
						break;
					else {
						Efirst[index] = true;
					}
				}
				nt = nt->next;
			}
		}
		t = t->next;
		count++;
	}
}

void FindFollowOne() {
	follow[0] = new FollowChar;
	follow[0]->followChar = '$';
	follow[0]->next = NULL;
	for (int i = 0; i < num; i++) {
		SpreadString*t = spread[i];
		while (t) {
			if (t->emptyAble) {
				t = t->next;
				continue;
			}
			NoneTerminal*nt = t->nonTerm;
			int lastIndex = -1;
			while (nt) {
				if ((nt->index == -1) && (lastIndex != -1)) {	//如果找到一个终结符，且前面一个是非终结符 则直接收取
					FollowChar*ft = new FollowChar;
					ft->followChar = nt->nonTerm[0];
					ft->next = follow[lastIndex];
					follow[lastIndex] = ft;
				}
				else {
					lastIndex = nt->index;
				}
				nt = nt->next;
			}
			t = t->next;
		}
	}
}

bool isin(FollowChar*f, char c) {
	if (!f)
		return false;
	while (f) {
		if (f->followChar == c)
			return true;
		f = f->next;
	}
	return false;
}

int FindFollowDFS(NoneTerminal*nt, int index) {
	int check = 0;
	if (nt->next) {
		check = FindFollowDFS(nt->next, index);
	}
	else {
		check = -1;	//结尾的位置
	}
	if (check == 1)	//结尾是一个终结符
		return 1;
	if (nt->index == -1) {	//结尾或当前是一个终结符
		return 1;
	}
	if (Efirst[nt->index] && (check == 2 || check == -1)) {	//结尾的非终结符可以推出空,状态延续条件是上次可推出空或者当前是最末尾
		FollowChar*fit = follow[index];	//将头的follow加入给当前位置的follow
		while (fit) {
			if (isin(follow[nt->index], fit->followChar)) {
				fit = fit->next;
				continue;
			}
			FollowChar*flt = new FollowChar;
			flt->followChar = fit->followChar;
			flt->next = follow[nt->index];
			follow[nt->index] = flt;
			fit = fit->next;
		}
		if (check != -1) {	//如果不是在最末尾
			int nextIndex = nt->next->index;
			FirstChar*fit = first[nextIndex];	//将下一个的first加入给当前位置的follow
			while (fit) {
				if (isin(follow[nt->index], fit->firstChar)) {
					fit = fit->next;
					continue;
				}
				FollowChar*flt = new FollowChar;
				flt->followChar = fit->firstChar;
				flt->next = follow[nt->index];
				follow[nt->index] = flt;
				fit = fit->next;
			}
		}
		return 2;
	}
	if (!Efirst[nt->index] && (check == 2 || check == -1 || check == 3)) {	//当前位置不能推空将导致状态陷入到3
		if (check == 2) {
			FollowChar*fit = follow[index];	//将头的follow加入给当前位置的follow
			while (fit) {
				if (isin(follow[nt->index], fit->followChar)) {
					fit = fit->next;
					continue;
				}
				FollowChar*flt = new FollowChar;
				flt->followChar = fit->followChar;
				flt->next = follow[nt->index];
				follow[nt->index] = flt;
				fit = fit->next;
			}
		}
		if (check != -1) {	//如果不是在最末尾
			int nextIndex = nt->next->index;
			FirstChar*fit = first[nextIndex];	//将下一个的first加入给当前位置的follow
			while (fit) {
				if (isin(follow[nt->index], fit->firstChar)) {
					fit = fit->next;
					continue;
				}
				FollowChar*flt = new FollowChar;
				flt->followChar = fit->firstChar;
				flt->next = follow[nt->index];
				follow[nt->index] = flt;
				fit = fit->next;
			}
		}
		else {
			FollowChar*fit = follow[index];	//将头的follow加入给当前位置的follow
			while (fit) {
				if (isin(follow[nt->index], fit->followChar)) {
					fit = fit->next;
					continue;
				}
				FollowChar*flt = new FollowChar;
				flt->followChar = fit->followChar;
				flt->next = follow[nt->index];
				follow[nt->index] = flt;
				fit = fit->next;
			}
		}
		return 3;
	}
}

void FindFollowTwo(int index) {
	SpreadString*t = spread[index];
	while (t) {
		if (t->emptyAble) {
			t = t->next;
			continue;
		}
		NoneTerminal*nt = t->nonTerm;
		FindFollowDFS(nt, index);	//处理一条spread
		t = t->next;
	}
}

void AddToTerm(char c) {
	bool check = false;
	for (int j = 0; j < numTem; j++) {
		if (terminal[j] == c) {
			check = true;
			break;
		}
	}
	if (!check) {
		terminal[numTem++] = c;
	}
}

int FindTermIndex(char c) {
	for (int j = 0; j < numTem; j++) {
		if (terminal[j] == c) {
			return j;
		}
	}
	return -1;
}

void GenCheckList() {
	terminal[numTem++] = '$';
	for (int i = 0; i < num; i++) {
		printf("First(%s) : ", nonTerminal[i]);
		if (Efirst[i])
			printf("&:%d ", Eindex[i]);
		FirstChar*t = first[i];
		while (t) {
			printf("%c:%d ", t->firstChar, t->spreadIndex);
			AddToTerm(t->firstChar);
			t = t->next;
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < num; i++) {
		printf("Follow(%s) : ", nonTerminal[i]);
		FollowChar*t = follow[i];
		while (t) {
			printf("%c ", t->followChar);
			AddToTerm(t->followChar);
			t = t->next;
		}
		printf("\n");
	}
	printf("\n");
	terminal[numTem++] = '&';
	//上面为term集的构造与first,follow集结果的输出
	for (int i = 0; i < num; i++)
		for (int j = 0; j < numTem; j++)
			cl[i][j].speIndex = -1;
	for (int i = 0; i < num; i++) {
		if (Efirst[i]) {	//如果可以为空，则把follow对应的符号置为空的生成式
			FollowChar*ft = follow[i];
			while (ft) {
				int index = FindTermIndex(ft->followChar);
				cl[i][index].speIndex = Eindex[i];
				ft = ft->next;
			}
		}
		FirstChar*t = first[i];
		while (t) {	//遍历first集，把每个元素的对应生成式加入cl
			AddToTerm(t->firstChar);
			int index = FindTermIndex(t->firstChar);
			cl[i][index].speIndex = t->spreadIndex;
			t = t->next;
		}
	}
}

int cStack[100];	//语法分析栈
int sTop = 0;	//栈顶

void AddNonEmpty(NoneTerminal*n) {
	if (n->next)
		AddNonEmpty(n->next);
	if (n->index == -1) {	//加入终结符
		cStack[sTop++] = FindTermIndex(n->nonTerm[0]) + num;
		return;
	}
	cStack[sTop++] = n->index;	//加入非终结符
}

void AddToCheckStack(SpreadString*s) {
	if (s->emptyAble) {	//加入空
		cStack[sTop++] = FindTermIndex('&') + num;
		return;
	}
	AddNonEmpty(s->nonTerm);
}

void PrintSpread(int index, SpreadString*t) {
	printf("%s -> ", nonTerminal[index]);
	if (t->emptyAble) {
		printf("&\n");
		return;
	}
	for (NoneTerminal*n = t->nonTerm; n; n = n->next) {
		printf("%s", n->nonTerm);
	}
	printf("\n");
}

void WriteError(char stack, char string) {
	if (stack != 'i'&&string == 'i'){
		printf("Unexpect ID\n");
		return;
	}
	if (stack == ')'){
		printf("Expect a ')'\n");
		return;
	}
	printf("Unexpect Error!!\n");
}

void WriteError1(int t, char string) {
	if (t == 2 && string == 'i') {
		printf("Unexpect ID\n");
		return;
	}
	if (t == 4) {
		printf("Expect a ID\n");
		return;
	}
	if (t == 3){
		printf("Expect a '*'\n");
	return;
	}
	if (t == 1){
		printf("Expect a '+'\n");
		return;
	}
	printf("Unexpect Error!!\n");
}

bool Check(char*s) {
	int len = strlen(s);
	s[len - 1] = '$';	//加入终结符
	s[len] = 0;
	cStack[sTop++] = num;	//首先压入终结符
	cStack[sTop++] = 0;	//再压入起始非终结符
	while (*s) {
		int ord = cStack[sTop - 1];
		if (ord >= num) {	//说明为终结符
			if (terminal[ord - num] == *s) {	//栈顶与式子内容相同
                printf("match %c\n", terminal[ord - num]);
				sTop--;
				s++;
				if (sTop == 0)	//栈空则为合法
					return true;
			}
			else if (terminal[ord - num] == '&') {	//栈顶为空，但式子不符合，则直接消掉
				sTop--;
			}
			else {
				WriteError(terminal[ord - num], *s);
				return false;	//否则非法
			}
		}
		else {	//说明为非终结符
			int index = FindTermIndex(*s);
			if (cl[ord][index].speIndex == -1) {
				WriteError1(index, *s);
				return false;	//未能从语法表中找到合适的项目，则为非法
			}
			int speIndex = cl[ord][index].speIndex;
			sTop--;
			SpreadString*t = spread[ord];
			while (speIndex) {	//找到对应的spread
				t = t->next;
				speIndex--;
			}
			PrintSpread(ord, t);
			AddToCheckStack(t);
		}
	}
}

int main() {
	char tmp[256];
	FILE*f = fopen("Data.txt", "r");
	while (!feof(f)) {
		memset(tmp, 0, sizeof(char) * 256);
		fgets(tmp, 256, f);
		if (!strcmp(tmp, "#"))
			break;
		deal(tmp);
	}
	fclose(f);
	findIndex();
	memset(finded, false, sizeof(bool)*num);
	memset(Efirst, false, sizeof(bool)*num);
	for (int i = 0; i < num; i++)
		if(!finded[i])
			DFS(i);
	memset(Efollow, false, sizeof(bool)*num);
	FindFollowOne();	//进行终结符吸收
	for (int i = 0; i < num; i++)
		FindFollowTwo(i);
	GenCheckList();

	f = fopen("Data2.txt", "r");	//读入待判断的符号流
	while (!feof(f)) {
		memset(tmp, 0, sizeof(char) * 256);
		fgets(tmp, 256, f);
		if (!strcmp(tmp, "#"))
			break;
		bool check = Check(tmp);
		if (check)
			printf("OK\n");
		printf("______________________________________________________\n");
	}
	fclose(f);
	system("pause");
}
