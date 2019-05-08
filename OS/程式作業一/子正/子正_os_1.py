import time
import threading
import multiprocessing

def bubbleSort( list_1 ):
  for i in range( len( list_1 ) - 1 , 0, -1 ): # len( list_1 ) - 1  to 1
    for j in range( i ): # 0 to i - 1
      if list_1[j] > list_1[j+1]:
        # swap
        tmp = list_1[j]
        list_1[j] = list_1[j+1]
        list_1[j+1] = tmp
        
  return list_1
# end bubbleSort()

def merge( list_1, list_2 ):
  result = []
  index_of_list_1 = 0
  index_of_list_2 = 0
  length = len( list_1 ) if len( list_1 ) > len( list_2 ) else len( list_2 )
  
  while( index_of_list_1 != len( list_1 ) and index_of_list_2 != len( list_2 ) ):
    if ( list_1[index_of_list_1] < list_2[index_of_list_2] ):
      result.append( list_1[index_of_list_1] )
      index_of_list_1 += 1
    else:
      result.append( list_2[index_of_list_2] )
      index_of_list_2 += 1
  # end while
  
  if index_of_list_1 == len( list_1 ): # list_1 is empty
    for index in range( index_of_list_2, len( list_2 ) ):
      result.append( list_2[index] )
  else: # index_of_list_2 == len( list_2 ) => list_2 is empty
    for index in range( index_of_list_1, len( list_1 ) ):
      result.append( list_1[index] )
  
  list_1[:] = []
  list_1 += result
  
  return list_1
#end merge()

if __name__ == '__main__':
  # input file
  file_name = input( "INPUT FILE NAME(NO FILE EXTENSION): " )
  input_file = open( file_name + '.txt', "r" )
  input_type = input_file.readline().split()[0]
  input_list = list( map( int, input_file.readline().split() ) )
  input_file.close()
  # input number of partitions
  if input_type != "1": 
    num_of_partitions = int( input( "DIVIDE THE DATA INTO HOW MANY PARTITIONS: ") )
    num_of_data = len( input_list ) // num_of_partitions # number of data per thread

  start = time.time() # start executing ###

  output_list = []
  if input_type == "1": # 1 process 1 partition
    bubbleSort( input_list )
    output_list.append( input_list )
  # end if
  elif input_type == "2": # multithreading with k partitions
    threads = []
    # divide the data into k partitions
    for i in range( num_of_partitions ):
      if ( i + 1 ) == num_of_partitions: # last partition
        output_list.append( input_list[ num_of_data * i : len( input_list ) ] )
      else:
        output_list.append( input_list[ num_of_data * i : num_of_data * (i+1) ] )
    # end for

    # create k threads and assign n/k data to each thread and do bubble sorting
    for i in range( num_of_partitions ):
      threads.append( threading.Thread( target = bubbleSort, args = ( output_list[i],) ) )
      threads[i].start()
    # end for
    
    # wait for all the threads finishing their job
    for i in range( num_of_partitions ):
      threads[i].join()
    # end for
    
    threads.clear()
    # merge the result
    for index in range( num_of_partitions ):
      # merge
      for i in range( 0, len( output_list ), 2 ): # 0, 2, 4, ... < len( output_list )
        if ( i + 1 ) < len( output_list ):
          threads.append( threading.Thread( target = merge, args = ( output_list[i], output_list[i+1],) ) )
          threads[i//2].start() # 0, 1, 2, ...
      # end for
      
      # wait for all the threads finishing their job
      for i in range( 0, len( output_list ), 2 ): # 0, 2, 4, ... < len( output_list )
        if ( i + 1 ) < len( output_list ): 
          threads[i//2].join() # 0, 1, 2, ...
      # end for
      
      # delete element with odd index
      for i in range( len( output_list ) ):
        if ( i + 1 ) < len( output_list ):
          del output_list[i+1]
      # end for
      
      threads.clear()
    # end for
  # end elif
  elif input_type == "3": # multiprocessing with k partitions
    # divide the data into k partitions
    for i in range( num_of_partitions ):
      if ( i + 1 ) == num_of_partitions: # last partition
        output_list.append( input_list[ num_of_data * i : len( input_list ) ] )
      else:
        output_list.append( input_list[ num_of_data * i : num_of_data * (i+1) ] )
    # end for
  
    # create k processes and assign n/k data to each process and do bubble sorting
    pool = multiprocessing.Pool( num_of_partitions )
    output_list = pool.map( bubbleSort, output_list )
    pool.close()
    pool.join()
    
    result = []
    # merge the result
    while( len( output_list ) != 1 ):
      # merge
      pool = multiprocessing.Pool( len( output_list ) // 2 )
      for i in range( 0, len( output_list ), 2 ): # 0, 2, 4, ... < len( output_list )
        if ( i + 1 ) < len( output_list ):
          result.append( pool.apply_async( merge, args = ( output_list[i], output_list[i+1], ) ) )
      # end for
      
      pool.close()
      pool.join()
      
      # assign result to even index
      for i in range( 0, len( output_list ), 2 ): # 0, 2, 4, ... < len( output_list )
        if ( i + 1 ) < len( output_list ):
          output_list[i] = result[i//2].get()
      # end for
      
      result.clear()
      
      # delete element with odd index
      for i in range( len( output_list ) ):
        if ( i + 1 ) < len( output_list ):
          del output_list[i+1]
      # end for
      
    # end while
  # end elif
  elif input_type == "4": # 1 process k partitions
    # divide the data into k partitions
    for i in range( num_of_partitions ):
      if ( i + 1 ) == num_of_partitions: # last partition
        output_list.append( input_list[ num_of_data * i : len( input_list ) ] )
      else:
        output_list.append( input_list[ num_of_data * i : num_of_data * (i+1) ] )
    # end for
        
    # bubble sort each partition
    for i in range( num_of_partitions ):
      bubbleSort( output_list[i] )
    # end for
    
    # merge the result
    for index in range( num_of_partitions ):
      # merge
      for i in range( 0, len( output_list ), 2 ): # 0, 2, 4, ... < len( output_list )
        if ( i + 1 ) < len( output_list ): 
          merge( output_list[i], output_list[i+1] ) 
      # end for
      
      # delete element with odd index
      for i in range( len( output_list ) ):
        if ( i + 1 ) < len( output_list ):
          del output_list[i+1]
      # end for
      
    # end for
  # end elif
  else: print( "INPUT TYPE MUST BE IN [1-4]" )

  end = time.time() # end executing ###

  # output the result
  
  output_list = sum( output_list, [] )
  output_file = open( file_name + '_output.txt', 'w' )
  output_file.write( "SORTING:\n" )
  for item in output_list: output_file.write( str( item ) + " " )
  output_file.write( "\nEXECUTION TIME: {:.6f} s".format( end - start ) )
  output_file.close()