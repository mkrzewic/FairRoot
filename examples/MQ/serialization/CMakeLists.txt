 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libSerializationExample" 

set(INCLUDE_DIRECTORIES
    ${BASE_INCLUDE_DIRECTORIES}
    ${CMAKE_SOURCE_DIR}/fairmq
    ${CMAKE_SOURCE_DIR}/fairmq/devices
    ${CMAKE_SOURCE_DIR}/fairmq/options
    ${CMAKE_SOURCE_DIR}/fairmq/tools
    ${CMAKE_SOURCE_DIR}/fairmq/nanomsg
    ${CMAKE_SOURCE_DIR}/fairmq/zeromq
    ${CMAKE_SOURCE_DIR}/base/MQ
    ${CMAKE_SOURCE_DIR}/base/MQ/devices
    ${CMAKE_SOURCE_DIR}/base/MQ/policies/Sampler
    ${CMAKE_SOURCE_DIR}/base/MQ/policies/Serialization
    ${CMAKE_SOURCE_DIR}/base/MQ/policies/Storage
    ${CMAKE_SOURCE_DIR}/base/MQ/baseMQtools
    ${CMAKE_SOURCE_DIR}/examples/MQ/serialization
    ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/data_generator
    ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/data_struct
    ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/src/1-simple/devices
    ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/src/2-multi-part/devices

)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZMQ_INCLUDE_DIR}
    ${GSL_INCLUDE_DIRS}
)



If(PROTOBUF_FOUND)
   set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_BINARY_DIR}
    ${INCLUDE_DIRECTORIES}
  )
  Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${PROTOBUF_INCLUDE_DIR}
  )
EndIf(PROTOBUF_FOUND)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(SCRIPT_ARGS "\${\@:2}")

# Data generator
configure_file( ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/data_generator/startMQSerializationGenerateData.sh.in ${CMAKE_BINARY_DIR}/bin/startMQSerializationGenerateData.sh )

# start MQ chains
configure_file( ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/scripts/startMQSerializationEx1.sh.in ${CMAKE_BINARY_DIR}/bin/startMQSerializationEx1.sh )
configure_file( ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/scripts/startMQSerializationEx2.sh.in ${CMAKE_BINARY_DIR}/bin/startMQSerializationEx2.sh )

# test scripts
configure_file( ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/test/startTestSerializationEx1.sh.in ${CMAKE_BINARY_DIR}/bin/startTestSerializationEx1.sh )
configure_file( ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/test/startTestSerializationEx2.sh.in ${CMAKE_BINARY_DIR}/bin/startTestSerializationEx2.sh )



# Create a default directory that will hold root input/output data file
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/data_io)

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
    ${GSL_LIBRARY_DIRS}
)

if(DDS_PATH)
  set(LINK_DIRECTORIES
    ${LINK_DIRECTORIES}
    ${DDS_PATH}/lib
  )
endif(DDS_PATH)

link_directories(${LINK_DIRECTORIES})

If(PROTOBUF_FOUND)
    add_custom_command(
        OUTPUT
          ${CMAKE_CURRENT_BINARY_DIR}/Tuto7Payload.pb.h
          ${CMAKE_CURRENT_BINARY_DIR}/Tuto7Payload.pb.cc
        COMMAND
          ${PROTOBUF_PROTOC_EXECUTABLE} -I=. --cpp_out=${CMAKE_CURRENT_BINARY_DIR} Tuto7Payload.proto
        WORKING_DIRECTORY
          ${CMAKE_SOURCE_DIR}/examples/MQ/serialization/data_struct
        )

Set(NO_DICT_SRCS
    ${NO_DICT_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/Tuto7Payload.pb.cc
  )
EndIf(PROTOBUF_FOUND)

Set(SRCS
    data_struct/MyDigi.cxx
    data_struct/MyHit.cxx
    data_struct/MyPodData.cxx
)


Set(LINKDEF SerializationExampleLinkDef.h)
Set(LIBRARY_NAME SerializationExample)

Set(DEPENDENCIES
    Base 
    FairMQ 
    BaseMQ 
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    Minuit
    XMLIO
    MathMore
    gsl
    gslcblas
)

If(PROTOBUF_FOUND)
    Set(DEPENDENCIES
      ${DEPENDENCIES}
      ${PROTOBUF_LIBRARY}
    )
Endif(PROTOBUF_FOUND)

GENERATE_LIBRARY()


Set(Exe_Names

    serialization-Ex1-Sampler
    serialization-Ex1-Processor
    serialization-Ex1-Sink

    serialization-Ex2-Sampler
    serialization-Ex2-Processor
    serialization-Ex2-Sink

    #genericMQTutoSamplerTest
    test-serialization-Ex1-Processor
    test-serialization-Ex1-sink

    test-serialization-Ex2-Processor
    test-serialization-Ex2-sink
)

set(Exe_Source
    src/1-simple/run/runEx1Sampler.cxx
    src/1-simple/run/runEx1Processor.cxx
    src/1-simple/run/runEx1Sink.cxx

    src/2-multi-part/run/runEx2Sampler.cxx
    src/2-multi-part/run/runEx2Processor.cxx
    src/2-multi-part/run/runEx2Sink.cxx

    #test/runSamplerT7Test.cxx
    test/testEx1Processor.cxx
    test/testEx1Sink.cxx

    test/testEx2Processor.cxx
    test/testEx2Sink.cxx

)

############################################################
# Check if we run on Ubuntu version>=11 and if it is the case
# add --no-as-needed flag to the linker.
# it is needed on Ubuntu xx>=11 to load Ttree in executables
# see : https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=14064
# Alternative is to instantiate a TApplication or load manually with gSystem->Load("libTree.so") in main file
set(UBUNTU_GREATERTHAN_v11_LINKER_FLAG )

if(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
    # use the LSB stuff if possible 
    EXECUTE_PROCESS(
       COMMAND cat /etc/lsb-release
       COMMAND grep DISTRIB_ID
       COMMAND awk -F= "{ print $2 }"
       COMMAND tr "\n" " "
       COMMAND sed "s/ //"
       OUTPUT_VARIABLE LSB_ID
       RESULT_VARIABLE LSB_ID_RESULT
    )
    EXECUTE_PROCESS(
       COMMAND cat /etc/lsb-release
       COMMAND grep DISTRIB_RELEASE
       COMMAND awk -F= "{ print $2 }"
       COMMAND tr "\n" " "
       COMMAND sed "s/ //"
       OUTPUT_VARIABLE LSB_VER
       RESULT_VARIABLE LSB_VER_RESULT
    )
    
    if(NOT ${LSB_ID} STREQUAL "")
        # found some, use it :D
        set(INSTALLER_PLATFORM "${LSB_ID}-${LSB_VER}" CACHE PATH "Installer chosen platform")
        if(${LSB_ID} STREQUAL "Ubuntu" AND ${LSB_VER} VERSION_GREATER "10.0")
            set(UBUNTU_GREATERTHAN_v11_LINKER_FLAG "-Wl,--no-as-needed")
        endif()
    else(NOT ${LSB_ID} STREQUAL "")
       set(INSTALLER_PLATFORM "linux-generic" CACHE PATH "Installer chosen platform")
    endif(NOT ${LSB_ID} STREQUAL "")
endif(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
############################################################



List(LENGTH Exe_Names _length)
Math(EXPR _length ${_length}-1)

ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES SerializationExample ${UBUNTU_GREATERTHAN_v11_LINKER_FLAG})
    GENERATE_EXECUTABLE()
EndForEach(_file RANGE 0 ${_length})



Set(EXE_NAME serialization-Ex-generate-data)
Set(SRCS data_generator/runGenerateData.cxx)
Set(DEPENDENCIES SerializationExample RooFitCore RooFit Foam )
GENERATE_EXECUTABLE()




add_test(NAME run_serialization_ex1 COMMAND ${CMAKE_BINARY_DIR}/bin/startTestSerializationEx1.sh)
set_tests_properties(run_serialization_ex1 PROPERTIES TIMEOUT "30")
set_tests_properties(run_serialization_ex1 PROPERTIES 
    PASS_REGULAR_EXPRESSION "Data generation successful"
    PASS_REGULAR_EXPRESSION "Sent 100 messages!" 
    PASS_REGULAR_EXPRESSION "Received 100 and sent 100 messages!"
    PASS_REGULAR_EXPRESSION "Received 100 messages!"
    )

add_test(NAME run_serialization_ex2 COMMAND ${CMAKE_BINARY_DIR}/bin/startTestSerializationEx2.sh)
set_tests_properties(run_serialization_ex2 PROPERTIES TIMEOUT "30")
set_tests_properties(run_serialization_ex2 PROPERTIES 
    PASS_REGULAR_EXPRESSION "Data generation successful"
    PASS_REGULAR_EXPRESSION "Sent 100 messages!" 
    PASS_REGULAR_EXPRESSION "Received 100 and sent 100 messages!"
    PASS_REGULAR_EXPRESSION "Received 100 messages!"
    )
set_tests_properties(run_serialization_ex2 PROPERTIES 
    DEPENDS run_serialization_ex1
    )




