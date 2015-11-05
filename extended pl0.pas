program pl0(input,output,fin) ;  {* version 1.0 oct.1989 *}
{* PL/0 compiler with code generation *}
const norw = 13;          {* no. of reserved words *}
      txmax = 100;        {* length of identifier table *}
      nmax = 14;          {* max. no. of digits in numbers *}
      al = 10;            {* length of identifiers *}
      amax = 2047;        {* maximum address *}
      levmax = 3;         {* maximum depth of block nesting *}
      cxmax = 200;        {* size of code array *}
      
type symbol =
     ( nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,lss,
       leq,gtr,geq,lparen,rparen,comma,semicolon,period,becomes,
       beginsym,endsym,ifsym,thensym,whilesym,dosym,callsym,constsym,
       varsym,procsym,readsym,writesym );{*reserved symbols or predefined symbols, Ok, these are pre-defined symbols*}
     alfa = packed array[1..al] of char; {*char array of identifier*}
     objecttyp = (constant,variable,procedure);{*this is object type which includes 3 basic type*}
     symset = set of symbol;
     fct = ( lit,opr,lod,sto,cal,int,jmp,jpc,red,wrt ); {* functions *}   {* I think they were instructions of the middle codes*}
     instruction = packed record    {*as I have say*}
                     f : fct;            {* function code *}
                     l : 0..levmax;      {* level *}
                     a : 0..amax;        {* displacement address *}
                   end;
                  {*   lit 0, a : load constant a
                      opr 0, a : execute operation a
                      lod l, a : load variable l,a
                      sto l, a : store variable l,a
                      cal l, a : call procedure a at level l
                      int 0, a : increment t-register by a
                      jmp 0, a : jump to a
                      jpc 0, a : jump conditional to a
                      red l, a : read variable l,a
                      wrt 0, 0 : write stack-top
                  *}   {*the usage of these instructions*}
                   {*variables used in the compiler code*}
var   ch : char;      {* last character read *}
      sym: symbol;    {* last symbol read *}
      id : alfa;      {* last identifier read *}
      num: integer;   {* last number read *}
      cc : integer;   {* character count *}
      ll : integer;   {* line length *}
      kk,err: integer; {*kk max length of a symbol,err no. of the error*}
      cx : integer;   {* code allocation index *}
      line: array[1..81] of char; {*OK, now we see, the max character num of a line is 81*}
      a : alfa; 
      code : array[0..cxmax] of instruction; {*max num of the code is 200*}
      word : array[1..norw] of alfa;	{*13 identifiers*}
      wsym : array[1..norw] of symbol;	{*13 symbols*}
      ssym : array[char] of symbol;	{*some charactor symbol,like ' ','^','+'*}
      mnemonic : array[fct] of
                   packed array[1..5] of char;
      declbegsys, statbegsys, facbegsys : symset;		{*3 symbolset, declare state factor begin symbol*}
      table : array[0..txmax] of	{*the max size of the identifier table is 100*}
                record	{*record in pascal*}
                  name : alfa;    {*two attribute of a table record*}
                  case kind: objecttyp of
                    constant : (val:integer );
                    variable,prosedure: (level,adr: integer )
                end;
      fin : text;     {* source program file *}
      sfile: string;  {* source program file name *}

procedure error( n : integer );  { P384 }
  begin
    writeln( '****', ' ':cc-1, '^', n:2 );
    err := err+1	{*now I know, err is the no. of the error*}
  end; { error }	
  
procedure getsym;  {*get a symbol*}
  var i,j,k : integer;
  procedure getch; {*get a char*}
    begin
      if cc = ll  {*get character to end of line*}
      then begin {*read next line*}
             if eof(fin)  {*function eof is used to judge if the input is over*}
             then begin
                    writeln('program incomplete');	{*if there is nothing in the file,print the file is incomplete*}
                    close(fin);
                    exit;
                  end;
             ll := 0;
             cc := 0;
	     write(cx:4,' ');  {*print code address*}
             while not eoln(fin) do  {*read a line into a line array*}
               begin
                 ll := ll+1;
                 read(fin,ch);
                 write(ch);
                 line[ll] := ch
               end;
             writeln;
             readln(fin);
             ll := ll+1;
             line[ll] := ' ' {*process end-line*}
           end;
      cc := cc+1;
      ch := line[cc]
    end; {*getch end,put all the charactors of a line into an array*}
  begin {* procedure getsym;  P384 *}
    while ch = ' ' do
      getch;
    if ch in ['a'..'z']
    then begin  {*identifier of reserved word*}
           k := 0;
           repeat
             if k < al {*put a word into the array "a",and the length of it should less than al, until there is a charactor which isn't belong to a symbol*}
             then begin
                    k := k+1;
                    a[k] := ch
                  end;
             getch
           until not( ch in ['a'..'z','0'..'9']); 
           if k >= kk        {*kk : last identifier length*}
           then kk := k
           else repeat
                  a[kk] := ' ';
                  kk := kk-1
                until kk = k;	{*i'm confused,why?*}
           id := a;
           i := 1;
           j := norw;   {*binary search reserved word table, binary search! diao!*}
           repeat
             k := (i+j) div 2;
             if id <= word[k]
             then j := k-1;
             if id >= word[k]
             then i := k+1
           until i > j;
           if i-1 > j
           then sym := wsym[k]
           else sym := ident
         end
    else if ch in ['0'..'9']
         then begin  {*number,put a number not into an array,but a number,multipy several 10 by the bits of the number*}
                k := 0;
                num := 0;
                sym := number;
                repeat
                  num := 10*num+(ord(ch)-ord('0'));
                  k := k+1;
                  getch
                until not( ch in ['0'..'9']);
                if k > nmax
                then error(30)
         end
    else if ch = ':'
         then begin
                getch;
                if ch = '='   {* ":=" *}
                then begin
                      sym := becomes;
                      getch
                     end
                else sym := nul {* no meaning *}
              end
   else if ch = '<'
        then begin
               getch;
               if ch = '='
               then begin
                      sym := leq; {* "<=" *}
                      getch
                    end
               else if ch = '>'
                    then begin
                           sym := neq;  {* "<>" *}
                           getch
                         end
               else sym := lss	{* "<" *}
             end
   else if ch = '>'
        then begin
               getch;
               if ch = '='
               then begin
                      sym := geq; {* ">=" *}
                      getch
                    end
               else sym := gtr	{* ">" *}
             end
   else begin
          sym := ssym[ch];  {* don't follow,maybe some specil charactor,for example '#'  *}
          getch
        end
  end; {* end of getsym *}
  
procedure gen( x: fct; y,z : integer ); {* generate the code. mabi! can you full spell it?!!! *}
  begin
    if cx > cxmax
    then begin
           writeln('program too long');
           close(fin);
           exit
         end;
    with code[cx] do {* "with a[ ] do x=1" == "a[i].x=1" with .. do is used to record var*}
      begin
        f := x;
        l := y;
        a := z
      end;
    cx := cx+1
  end; {*gen*}
  
procedure test( s1,s2 :symset; n: integer );  {*while a symbol is not found in the symbol set,print an error and the keep compiling*}
  begin
    if not ( sym in s1 )
    then begin
           error(n);
           s1 := s1+s2;
           while not( sym in s1) do  {*keep compiling,starting next one*}
             getsym
           end
  end; {*test*}
  
procedure block( lev,tx : integer; fsys : symset ); {*level:layer in the program,tx:pointer in the symbol table,fsys:group of words used for error handling*}
  var  dx : integer;  {*data allocation index*}
       tx0: integer;  {*initial table index*}
       cx0: integer;  {*initial code index*}

  procedure enter( k : objecttyp ); 
    begin  {*enter object into table*}
      tx := tx+1;{*pointer in the symbol table *}
      with table[tx] do {*put the symbol into the table*}
        begin
          name := id;
          kind := k;
          case k of
            constant : begin
                         if num > amax {*WHY AMAX?*}
                         then begin
                                error(30);
                                num := 0
                              end;	
                         val := num
                       end;
            variable : begin	{*if it's a variable*}
                         level := lev;
                         adr := dx;
                         dx := dx+1
                       end;
            prosedure: level := lev;	{*if it's a procedure*}
          end
        end
    end; {*enter*}

  function position ( id : alfa ): integer; {*find the position of the id*}
    var i : integer;
    begin
      table[0].name := id;{*WHY?*}
      i := tx;
      while table[i].name <> id do{*start find from the current position,or the last item in the table*}
        i := i-1;
      position := i
    end;  {*position*}
    
  procedure constdeclaration; {*declare a constant*}
    begin
      if sym = ident	{*ident? indentity?*}
      then begin
             getsym;
             if sym in [eql,becomes] {*become ":=" eql "=" -_-||*}
             then begin
                    if sym = becomes
                    then error(1);
                    getsym;
                    if sym = number {*OK, it's clear, this is the value of the const symbol*}
                    then begin
                           enter(constant);{*put the constant into the table*}
                           getsym
                         end
                    else error(2)
                  end
             else error(3)
           end
      else error(4)
    end; {*constdeclaration*}
    
  procedure vardeclaration; {*put an var into the table*}
    begin
      if sym = ident
      then begin	{* wocao!so direct! NO becomes?*}
             enter(variable);
             getsym
           end
      else error(4)
    end; { vardeclaration }
    
  procedure listcode;  {**}
    var i : integer;
    begin

      for i := cx0 to cx-1 do {*cx0 start of the current level,cx-1 before current code*}
        with code[i] do
          writeln( i:4, mnemonic[f]:7,l:3, a:5)	{*print code*}
    end; {*listcode*}
    
  procedure statement( fsys : symset ); {*Ok, a statement,<statement>::=<becomes expression>|<condition>|...*}
    var i,cx1,cx2: integer;
    procedure expression( fsys: symset); {*an expression,<expression>::=[+-]<term>{1,n}*}
      var addop : symbol;	{*'+'*}
      procedure term( fsys : symset);     {*a term*}
        var mulop: symbol ;	{*'*'*}
        procedure factor( fsys : symset ); {*min-item,fator*}
          var i : integer;
          begin
            test( facbegsys, fsys, 24 ); 	{*if the symbol not in facbegsys,error*}
            while sym in facbegsys do
              begin
                if sym = ident	{*if the symbol is an identity*}
                then begin
                       i := position(id);
                       if i= 0	{*wocao!Now i know why in the enter procedure,put the symbol in the 0 position*}
                       then error(11)
                       else
                         with table[i] do	{*get the symbol out from the symbol table*}
                           case kind of
                             constant : gen(lit,0,val);
                             variable : gen(lod,lev-level,adr);
                             prosedure: error(21)
                           end;
                       getsym
                     end
                else if sym = number	{*the sym is a number,Now i'm doubt with the meaning of the AMAX.*}
                     then begin
                            if num > amax
                            then begin
                                   error(30);
                                   num := 0
                                 end;
                            gen(lit,0,num);	{*put a number on the stack*}
                            getsym
                          end
                     else if sym = lparen	{*lparen=='('*}
                          then begin
                                 getsym;
                                 expression([rparen]+fsys);	{*recurrency there is a relation factor:='('expression')'*}
                                 if sym = rparen {*')'*}
                                 then getsym
                                 else error(22)
                               end;
                test(fsys,[lparen],23)
              end
          end; {*factor*}
        begin {*procedure term( fsys : symset);   P388
                var mulop: symbol ;*}
          factor( fsys+[times,slash]); 
          while sym in [times,slash] do	{*times '*' slash '/'*}
            begin
              mulop := sym;
              getsym;
              factor( fsys+[times,slash] );
              if mulop = times
              then gen( opr,0,4 ){*generate pcode which include '*' or '/'*}
              else gen( opr,0,5)
            end
        end; {*term*}
      begin {* procedure expression( fsys: symset);  P388
              var addop : symbol; *}
        if sym in [plus, minus]	{*expression:=[+|-]<term>{[+|-]<term>}*}
        then begin {*first symbol is + or -*}
               addop := sym;
               getsym;
               term( fsys+[plus,minus]);
               if addop = minus
               then gen(opr,0,1)
             end
        else term( fsys+[plus,minus]);	{*first symbol is a term*}
        while sym in [plus,minus] do	{*while the expression is not over*}
          begin	{*it's a little like term*}
            addop := sym;
            getsym;
            term( fsys+[plus,minus] );
            if addop = plus
            then gen( opr,0,2)
            else gen( opr,0,3)
          end
      end; {*expression*}
      
    procedure condition( fsys : symset ); {*now condition expression, one item in the composure of statement*}
    var relop : symbol;{*<condition>::=<exp><op><iden>|odd <exp>*}
      begin
        if sym = oddsym
        then begin
               getsym;
               expression(fsys);
               gen(opr,0,6)
             end
        else begin{*first condition of condition ^_^*}
               expression( [eql,neq,lss,gtr,leq,geq]+fsys);
               if not( sym in [eql,neq,lss,leq,gtr,geq])
               then error(20)
               else begin
                      relop := sym;
                      getsym;
                      expression(fsys);
                      case relop of
                        eql : gen(opr,0,8);
                        neq : gen(opr,0,9);
                        lss : gen(opr,0,10);
                        geq : gen(opr,0,11);
                        gtr : gen(opr,0,12);
                        leq : gen(opr,0,13);
                      end
                    end
             end
      end; {*condition*}
    begin { procedure statement( fsys : symset );  P389
            var i,cx1,cx2: integer; }{*zhegemeng! so complex!<-_->*}
      if sym = ident	{*assignment statement,first sumbol should be a identity*}
      then begin
             i := position(id);
             if i= 0
             then error(11)
             else if table[i].kind <> variable
                  then begin {*giving value to non-variation,heheda!*}
                         error(12);
                         i := 0
                       end;
             getsym;
             if sym = becomes	{*yes,become statement!*}
             then getsym
             else error(13);
             expression(fsys);	{*A := B ,this is B*}
             if i <> 0
             then
               with table[i] do
                 gen(sto,lev-level,adr)
           end
      else if sym = callsym	{*i think that's calling a procedure*}
      then begin
             getsym;
             if sym <> ident	{* *__*, why don't use a '!='*}
             then error(14)
             else begin
                    i := position(id);
                    if i = 0
                    then error(11)
                    else
                      with table[i] do
                        if kind = prosedure
                        then gen(cal,lev-level,adr) {*now call the procedure*}
                        else error(15);
                    getsym
                  end
           end
      else if sym = ifsym	{*Oh,shit,condition statement is coming*}
           then begin
                  getsym;
                  condition([thensym,dosym]+fsys);{*judge condition statement*}
                  if sym = thensym
                  then getsym
                  else error(16);
                  cx1 := cx;
                  gen(jpc,0,0);    {*generate a jmp instruction*}
                  statement(fsys);
                  code[cx1].a := cx 
                end
      else if sym = beginsym	{* fuhe statement start with 'begin'*}
           then begin
                  getsym;
                  statement([semicolon,endsym]+fsys);{*enter a statement*}
                  while sym in ([semicolon]+statbegsys) do{*there still remain some statements*}
                     begin
                       if sym = semicolon
                       then getsym
                       else error(10);
                       statement([semicolon,endsym]+fsys)
                     end;
                  if sym = endsym {*OK. It's over*}
                  then getsym
                  else error(17)
                end
      else if sym = whilesym	{*now while statement*}
           then begin
                  cx1 := cx;	{*pay attention!it's a label*}
                  getsym;
                  condition([dosym]+fsys);
                  cx2 := cx;	{*second label,do's start*}
                  gen(jpc,0,0);{*generate jump instruction*}
                  if sym = dosym
                  then getsym
                  else error(18);
                  statement(fsys);
                  gen(jmp,0,cx1);	{*excute the judge statement*}
                  code[cx2].a := cx	{*soga, pay attention,code[cx2]=='jpc,0,0',yes,that's it,so,when you change code[cx2].a,it will be code[cx2]='jpc,0,cx',wonderful!*}
                end
      else if sym = readsym {*that's read statement!*}
           then begin
                  getsym;
                  if sym = lparen {* '(' *}
                  then
                    repeat
                      getsym;
                      if sym = ident
                      then begin
                             i := position(id);  {*check if exist in the table*}
                             if i = 0
                             then error(11)
                             else if table[i].kind <> variable
                             then begin
                                    error(12);
                                    i := 0
                                  end
                             else with table[i] do	{*read a value into the var*}
                                  gen(red,lev-level,adr)
                           end
                      else error(4);
                      getsym;
                    until sym <> comma	{*there is no comma,there is no variables*}
                  else error(40);
                  if sym <> rparen	{*no ')', wrong!*}
                  then error(22);
                  getsym
           end
      else if sym = writesym	{*now write!*}
           then begin
                  getsym;
                  if sym = lparen
                  then begin
                         repeat
                           getsym;
                           expression([rparen,comma]+fsys);	{*OK,write statement is composed by expression*}
                           gen(wrt,0,0);
                         until sym <> comma;
                         if sym <> rparen
                         then error(22);
                         getsym
                       end
                  else error(40)
                end;
      test(fsys,[],19)
    end; {*statement,finally end*}
  begin  {* procedure block( lev,tx : integer; fsys : symset );    P390
                var  dx : integer;  /* data allocation index */
                     tx0: integer;  /*initial table index */
                     cx0: integer;  /* initial code index */ *}
    dx := 3;{*there should be at least 3 empty location used to store 3 address,sl static chain,dl denamic chain,return address Ra*}
    tx0 := tx;{*table pointer pointing to the address of current level in the table*}
    table[tx].adr := cx;{*there will be used when this declare part is used,current address should be store in the symbol table*}
    gen(jmp,0,0); {*jump from declaration part to statement part,in the last it's changed to the start part of statement*}
    if lev > levmax
    then error(32);

    repeat  {*the niubiest part is start!*}
      if sym = constsym	{*declare const value*}
      then begin
             getsym;
             repeat
               constdeclaration;
               while sym = comma do
                 begin
                   getsym;
                   constdeclaration
                 end;
               if sym = semicolon   {*';' is the symbol of ending constdeclare*}
               then getsym
               else error(5)
             until sym <> ident
           end;
      if sym = varsym	{*declare variable*}
      then begin
             getsym;
             repeat
               vardeclaration;
               while sym = comma do
                 begin
                   getsym;
                   vardeclaration
                 end;
               if sym = semicolon	{*like above,';' means stop var declaring*}
               then getsym
               else error(5)
             until sym <> ident;
           end;
      while sym = procsym do	{*a little complex,enter a procedure,maybe not only one procedure*}
        begin
          getsym;
          if sym = ident
          then begin
                 enter(prosedure);
                 getsym
               end
          else error(4);
          if sym = semicolon
          then getsym
          else error(5);
          block(lev+1,tx,[semicolon]+fsys);{*new block is starting*}
          if sym = semicolon
          then begin
                 getsym;
                 test( statbegsys+[ident,procsym],fsys,6){*check if the current symbol set is correct*}
               end
          else error(5)
        end;
      test( statbegsys+[ident],declbegsys,7)
    until not ( sym in declbegsys );{*declare part is over*}
    code[table[tx0].adr].a := cx;  {*back enter statement code's start adr.Yes,i have seen*}
    with table[tx0] do
      begin
        adr := cx; {*code's start address,yes,statement part start*}
      end;
    cx0 := cx;
    gen(int,0,dx); {*topstack point to operation area*}
    statement( [semicolon,endsym]+fsys);{*statement part,excute the statement in the block*}
    gen(opr,0,0); {*return*}
    test( fsys, [],8 );
    listcode;{*list the pcode!*}
  end {*block*};
  
procedure interpret;  {* interpretor!!!!! *}
  const stacksize = 500;{* 500 stack unit*}
  var p,b,t: integer; {*program-(instruction pointer),base-(data space allocate),topstack-register(top stack, this register is used to remember the top stack)*}
      i : instruction;{*instruction register,store the instruction which is going to exec*}
      s : array[1..stacksize] of integer; {*data store stack*}
  {*this function is used to calculate the address of fundamental address of data stack*}
  function base( l : integer ): integer;  {*l:current level-data_stack_level*}
    var b1 : integer;
    begin {*find base l levels down*}
      b1 := b;
      while l > 0 do
        begin
          b1 := s[b1];{*one level insert into one level,so we should find the inserted level form it's embeded level*}
          l := l-1
        end;
      base := b1 {*return the founded address*}
    end; {*base*}
  begin  {*the program is start*}
    writeln( 'START PL/0' );
    t := 0;
    b := 1;{*data stack address should be 1*}
    p := 0;
    s[1] := 0;
    s[2] := 0;
    s[3] := 0;{*all others should be inited to 0*}
    repeat {*start excute*}
      i := code[p]; {*get the instruction*}
      p := p+1;
      with i do
        case f of	{*now, these are the instructions*}
          lit : begin
                  t := t+1;{*store a commen value on the top of the stack*}
                  s[t]:= a;{*put it on the stack*}
                end;
          opr : case a of {*operator,behave different according to the value of a*}
                  0 : begin {*return from a function;}
                        t := b-1;	{*release the stack and data in this level*}
                        p := s[t+3];	{*get the value of RA,store it to p*}
                        b := s[t+2];	{*return the code which call the function*}
                      end;
                  1 : s[t] := -s[t];{*the item on the top of the stack,get the neg value of it*}
                  2 : begin{*add,pop the stack top*}
                        t := t-1;
                        s[t] := s[t]+s[t+1]
                      end;
                  3 : begin{*sub,*}
                        t := t-1;
                        s[t] := s[t]-s[t+1]
                      end;
                  4 : begin
                        t := t-1;
                        s[t] := s[t]*s[t+1]
                      end;
                  5 : begin
                        t := t-1;
                        s[t] := s[t]div s[t+1]
                      end;
                  6 : s[t] := ord(odd(s[t])); {*judge if the item on the top os stack is odd*}
                  8 : begin
                        t := t-1;
                        s[t] := ord(s[t]=s[t+1]){*judge if the top two value is equal,and pop the top of stack*}
                      end;
                  9 : begin
                        t := t-1;
                        s[t] := ord(s[t]<>s[t+1]){*judge if not equal*}
                      end;
                  10: begin
                        t := t-1;
                        s[t] := ord(s[t]< s[t+1])
                      end;
                  11: begin
                        t := t-1;
                        s[t] := ord(s[t] >= s[t+1])
                      end;
                  12: begin
                        t := t-1;
                        s[t] := ord(s[t] > s[t+1])
                      end;
                  13: begin
                        t := t-1;
                        s[t] := ord(s[t] <= s[t+1])
                      end;
                end;
          lod : begin	{*load the value of the var to the top of the stack*}
                  t := t+1;
                  s[t] := s[base(l)+a]	{*find the address of the data level,and add the offset*}
                end;
          sto : begin	{*the opp instruction of the above*}
                  s[base(l)+a] := s[t];  {*writeln(s[t]);*}
                  t := t-1
                end;
          cal : begin  {*generate new block mark*}
                  s[t+1] := base(l);{*push the sl to the stack top*}
                  s[t+2] := b;{*push the address of the current data stack*}
                  s[t+3] := p;{*push the RA*}
                  b := t+1;
                  p := a;{*jump to a*}
                end;
          int : t := t+a;{*allocate a space,size:a*}
          jmp : p := a;
          jpc : begin
                  if s[t] = 0	{*judge the current top stack value, if it were 0,jump*}
                  then p := a;
                  t := t-1;
                end;
          red : begin	{*read instruction,read a value to the var*}
                  writeln('??:');
                  readln(s[base(l)+a]);
                end;
          wrt : begin	{*write the value on the stack top out*}
                  writeln(s[t]);
                  t := t+1
                end
        end { with,case }
    until p = 0;
    writeln('END PL/0');
  end; {*interpret,niubi!*}

begin { main }
  writeln('please input source program file name : ');
  readln(sfile);
  assign(fin,sfile);{"set the input source to the fin"}
  reset(fin);
  for ch := 'A' to ';' do
    ssym[ch] := nul;{*init the set which will be used*}
  word[1] := 'begin        '; word[2] := 'call         ';
  word[3] := 'const        '; word[4] := 'do           ';
  word[5] := 'end          '; word[6] := 'if           ';
  word[7] := 'odd          '; word[8] := 'procedure    ';
  word[9] := 'read         '; word[10]:= 'then         ';
  word[11]:= 'var          '; word[12]:= 'while        ';
  word[13]:= 'write        ';
  
  wsym[1] := beginsym;      wsym[2] := callsym;
  wsym[3] := constsym;      wsym[4] := dosym;
  wsym[5] := endsym;        wsym[6] := ifsym;
  wsym[7] := oddsym;        wsym[8] := procsym;
  wsym[9] := readsym;       wsym[10]:= thensym;
  wsym[11]:= varsym;        wsym[12]:= whilesym;
  wsym[13]:= writesym;
  
  ssym['+'] := plus;        ssym['-'] := minus;
  ssym['*'] := times;       ssym['/'] := slash;
  ssym['('] := lparen;      ssym[')'] := rparen;
  ssym['='] := eql;         ssym[','] := comma;
  ssym['.'] := period;
  ssym['<'] := lss;         ssym['>'] := gtr;
  ssym[';'] := semicolon;
  
  mnemonic[lit] := 'LIT  '; mnemonic[opr] := 'OPR  ';
  mnemonic[lod] := 'LOD  '; mnemonic[sto] := 'STO  ';
  mnemonic[cal] := 'CAL  '; mnemonic[int] := 'INT  ';
  mnemonic[jmp] := 'JMP  '; mnemonic[jpc] := 'JPC  ';
  mnemonic[red] := 'RED  '; mnemonic[wrt] := 'WRT  ';
  
  declbegsys := [ constsym, varsym, procsym ];
  statbegsys := [ beginsym, callsym, ifsym, whilesym];
  facbegsys := [ ident, number, lparen ];
  err := 0;
  cc := 0;
  cx := 0;
  ll := 0;
  ch := ' ';
  kk := al;
  getsym;
  block( 0,0,[period]+declbegsys+statbegsys );{*now start!*}
  if sym <> period
  then error(9);
  if err = 0
  then interpret	{*now interpret!*}
  else write('ERRORS IN PL/0 PROGRAM');
  writeln;
  close(fin)
end.

{*sodesinai!!!!!!!!!a compiler just soso~~~~~~*}
