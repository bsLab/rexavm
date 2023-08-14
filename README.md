# rexavm

Tiny Machine Learning Virtualization for IoT and Edge Computing using the REXA VM

rexavm: "Real-time capable execution with Artificial Neural Network support" Virtual Machine

# Abstract

Tiny Machine Learning is a new approach that is being used for data-driven prediction classification and regression on microcontrollers using local sensor data. The models are typically learned off-line and sent to the microcontroller for use as binary objects or frozen and converted static data. This approach is not universal or flexible. The REXA VM can virtualize embedded systems and sensor nodes and includes a general machine learning framework that supports arbitrary dynamic artificial neural network and decision tree models. The models are delivered as text files with highly compressed program code that are enclosed in code frames with embedded data (model parameters). The VM offers fundamental computations for ANN, CNN, and DT models (Microservices). 

The REXAVM is highly customizable through the use of VM program code generators at compile time and incremental code processing at run time. The VM uses an integrated, highly efficient just-in-time compiler to create Bytecode from text code.

Source code templates are currently provided  for the C programming language. Supported host platforms are currently generic PCs and STM32 ARM Cortex microcontrollers.

The **novelties** are:

1. Unified and customizable software and hardware implementations of the VM (H/W co-design at the architecture level), including operational equivalent simulators;
2. Highly customizable machine Instruction Set Architecture (ISA);
3. Just-in-time text-to-bytecode compiler (always bound to VM);
4. Security by design, i.e., tasks can be isolated without overhead or interference;
5. Portability and deployment in highly heterogeneous environments using various VM modes: VM can be used as a co-processor, embedded in any host application, or used independently to implement a complete sensor or computational node.
6. Low-resource requirements, optimized for resource sharing, e.g., reusing an ADC sample buffer for computations from VM programming level, fast compared with other VMs; no data heap with memory management required;
7. Real-time capable VM due to multi-tasking, predictable instruction processing times, and micro-slicing with seamless integration in higher-level IO service loops;
8. An extensible and application-specific VM architecture is created by user-customizable and parameterizable VM code generation from code snippets and JSON configuration files, introducing a complete Hardware-Software-Simulation (HSS) co-design framework that supports the exploration of the design space under hard constraints;
9. A sensor processing and Tiny ML API supporting highly compact Artificial Neural Networps and decision tree models (inference only).


# Build 

Build instructions are in the sources folder.


