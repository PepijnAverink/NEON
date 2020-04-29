# NEON
NEON is a learning project to learn low-level rendering API's and their internals. Its purpose is to abstract API's in common rule set so the API backend can be interchanged. This project is still in early stages of development, and not everything will be working yet. Check back regularly for updates. 

### Supported platforms:
* Windows 32bit & 64bit

### Supported API's:
* DirectX12
* Vulkan

## Build Notes

1. Clone the repository
```cpp
git clone git@github.com:PepijnAverink/NEON.git
```
2. Check compiler versions

3. Installing Vulkan SDK

## Recently added
* VertexBuffer abstraction for DirectX12
* IndexBuffer abstraction for DirectX12

## Getting started


## Documentation

**Note:** This project is still a work in progress and the documentation listed below may get out of date or change as the project progresses.

### API Selection & GraphicsContext
The user can select which graphics API to use by creating a **GraphicsDriver** object. This a global object used by the project to check which API to use, and its not interacted with by the user untill the object is destroyed.

Possible values for GraphicsAPI parameter:
* DIRECTX12
* VULKAN

```cpp
// Creating a new GraphicsDriver
Graphics::GraphicsDriver::Create(Graphics::GraphicsAPI);

...

// Destroying GraphicsDriver object
Graphics::GraphicsDriver::Destroy();
```


### CommandPool
A **CommandPoolDescriptor** serves as a descriptor for the *CommandPool* object and is used on its creation.
The purpose of the _Name_ is purely for debugging, it will store a user friendly name in the obejct itself (Only stored in _Debug_) and this _Name_ will also show up in a graphics debugger. 
The _Type_ has a default fallback so the user does not have to specify anything for a default CommandPool object. However for clarity's sake it is always adviced to specify the type of CommandBuffer this CommandPool will used with.

A **CommandPool** object serves as the storage for your commands recorded with a *CommandBuffer* object. The CommandPool object is created using a CommandPoolDescriptor.
A single CommandPool object can be used for multiple CommandBuffer objects, but only one CommandBuffer object created with the same CommandPool can be in a recording state at a time.
A CommandPool can only be used to create a CommandBuffer that has the same _CommandBufferType_.

**Note:** The *CommandPool* object may store a reference to a CommandBuffer object and this object may not be properly released untill the CommandPool object is released. However it is completely safe to delete a CommandBuffer object while its CommandPool is still in use.

**Note:** A *CommandPool* object should not be deleted while a CommandBuffer is still recording or while executing a CommandBuffer object using this CommandPool.

**Creation:**
```cpp
CommandPool* m_CommandPool;
```
```cpp
// Setup pool descriptor
CommandPoolDescriptor poolDesc = {};
poolDesc.Name		 = "Main-CommandPool";
poolDesc.Type		 = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;

// Create command pool
m_CommandPool		 = CommandPool::Create(&poolDesc);
```

A *CommandPool* object can safely be reset after executing all the CommandBuffer objects using the CommandPool and synching using a **Fence** object. CommandPool memory is often dynamically allocated so not resseting it will cause significant increase in memory usage.

**Reset:**
```cpp
// Reset CommandPool
m_CommandPool->Reset();
```

When the CommandPool object has been created by the user, the user has ownership over the object and needs to take care of its dealocation. The CommandPool object is internally created using a ___new___ call and should be dealocated with a ___delete___.

**Dealocation:**
```cpp
// Dealocate CommandPool
delete m_CommandPool;
```
### CommandBuffer
A **CommandBufferDescriptor** serves as a descriptor for the *CommandBuffer* object and is used on its creation.
The purpose of the _Name_ is purely for debugging, it will store a user friendly name in the obejct itself (Only stored in _Debug_) and this _Name_ will also show up in a graphics debugger.
The _Type_ has a default fallback so the user only has to give a pointer to the *CommandPool* object for a default CommandBuffer object. The pointer to the CommandPool object passed in the descriptor is used to ___initialize___ and ___reset___ the CommandBuffer object.

A **CommandBuffer** object is used to issue commands to a *CommandQueue* object. The CommandBuffer object is created using a CommandBufferDescriptor.
On createion, the CommandBuffer object is in a non-recording state and should be handled by the user before any commands can be recorded.

**Creation:**
```cpp
CommandBuffer* m_CommandBuffer;
```
```cpp
// Setup commandBuffer descriptor
CommandBufferDescriptor commandBufferDesc = {};
commandBufferDesc.Name		  = "Main-CommandPool";
commandBufferDesc.Type		  = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
commandBufferDesc.CommandPool = m_CommandPool;

// Create command pool
m_CommandBuffer		 		  = CommandBuffer::Create(&commandBufferDesc);
```

Before the CommandBuffer can be executed using a CommandQueue object, the user needs to put it to a non-recording state. The state handling of the CommandBuffer object is all handled by the user.

```cpp
// Set CommandBuffer to recording state
m_CommandBuffer->StartRecording();

...

// End CommandBuffer recording
m_CommandBuffer->EndRecording();
```

Before a CommandBuffer object can be used to issue commands it is wise to reset the memory used by the CommandPool, this can either be done resetting the entire CommandPool or by resseting the CommandBuffer object itself.
This user should only reset the memory of a CommandPool if no CommandBuffers connected to the CommandPool are in a recording state or currently executing, the user can use a fence to check for the latter.

```cpp
// Reset CommandBuffer
m_CommandBuffer->Reset();
```

When the CommandBuffer object has been created by the user, the user has ownership over the object and needs to take care of its dealocation. The CommandBuffer object is internally created using a ___new___ call and should be dealocated with a ___delete___.

**Dealocation:**
```cpp
// Dealocate CommandBuffer
delete m_CommandBuffer;
```

### CommandQueue