# NEON

## Build Notes


## Documentation

**Note:** This project is still a work in progress and the documentation listed below may get out of date or change as the project progresses.

### CommandPool
A **CommandPoolDescriptor** Serves as a descriptor for the *CommandPool* object and is used on it's creation.
The purpose of the _Name_ is purely for debugging, it will store a user friendly name in the obejct itself (Only stored in _Debug_) and this _Name_ will also show up in a graphics debugger. 
The _Type_ has a default fallback so the user does not have to specify anything for a default *CommandPool* object. However for clarity's sake it is always adviced to specify the type of *CommandBuffer* this *CommandPool* will used with.

A **CommandPool** object serves as the storage for your commands recorded with a *CommandBuffer* object. The *CommandPool* object is created using a *CommandPoolDescriptor*.
A single *CommandPool* object can be used for multiple *CommandBuffer* objects, but only one *CommandBuffer* object created with the same *CommandPool* can be in a recording state at a time.
A *CommandPool* can only be used to create a *CommandBuffer* that has the same _CommandBufferType_

**Note:** The *CommandPool* object may store a reference to a *CommandBuffer* object and this object may not be properly released untill the *CommandPool* object is released. However it is completely safe to release a *CommandBuffer* object while it's *CommandPool* is still in use.

**Creation:**
```cpp
// Setup pool descriptor
CommandPoolDescriptor poolDesc = {};
poolDesc.Name		 = "Main-CommandPool";
poolDesc.Type		 = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;

// Create command pool
m_CommandPool		 = CommandPool::Create(&poolDesc);
```


### CommandBuffer

### CommandQueue