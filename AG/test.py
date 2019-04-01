


LOW     = 1
MIDDLE  = 2
HIGH    = 3
HIGHEST = 4
class Stack:

  def __init__( self ):
    self.S = []

  def isEmpty( self ):
    return ( self.S == [] )

  def Push( self, key ):
    self.S.append( key )

  def Pop( self ):
    if ( self.isEmpty() ):
      return "Error"
    else:
      return self.S.pop()

  def Display( self ):
    print( "Stack:", end = "" )
    print( self.S )

def priority_op( op ):
  if ( op == '^' ):
    return HIGHEST
  elif ( op == '*' or op == '/' ):
    return HIGH
  elif ( op == '+' or op == '-' ):
    return MIDDLE
  else:
    return LOW

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass
 
    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass
 
    return False

def infix_to_postfix( in_str ):
  k = len( in_str )
  S = Stack()
  out_str = ""

  for i in range( k ):
    ch = in_str[i]

    if ( ch in '+-*/^' ):
      done = False
      while ((S.isEmpty() == False) and (done == False)):
        top = S.Pop()
        if ( top == "Error" ):
          return "Error"
        elif ( ( top != '(' ) and ( priority_op( top ) >= priority_op( ch ) ) ):
          out_str += top
        else:
          S.Push( top )
          done = True
      S.Push( ch )

    elif ( ch == '(' ):
      S.Push( ch )

    elif ( ch == ')' ):
      top = S.Pop()
      if ( top == "Error" ):
        return "Error"
      while ( top != '(' ):
        out_str += top
        top = S.Pop()
        if ( top == "Error" ):
          return "Error"
    else:  # Operands
      out_str += ch

  while( S.isEmpty() == False ):
    out_str = out_str + S.Pop()

  return (out_str)

def Evaluate(postfix):
  k = len(postfix)
  S = Stack()
  try:
    for i in range(k):
      ch = postfix[i]
      if (ch in '+-*/^'):
        print("f1")
        top1 = float( S.Pop() )
        top2 = float( S.Pop() )
        print("f2")
        if (ch == '+'):
         top1 = top1 + top2
        elif (ch == '-'):
         top1 = top1 - top2
        elif (ch == '*'):
          top1 = top1 * top2
        else:
          top1 = top1 / top2
        S.Push( top1 )
      else:
        S.Push(ch)

    output = S.Pop()

    if ( S.isEmpty() == False ):
     output = "Error"
  except:
    output = "Error"

  return (output)





line = input()
while line != '0':
  output = infix_to_postfix(line)
  print(output)

  line = input()