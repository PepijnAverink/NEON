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

### API Selection



### CommandPool
A **CommandPoolDescriptor** Serves as a descriptor for the *CommandPool* object and is used on its creation.
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

### CommandQueue