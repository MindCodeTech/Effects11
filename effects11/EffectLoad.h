//--------------------------------------------------------------------------------------
// File: EffectLoad.h
//
// Direct3D 11 Effects header for the FX file loader
// A CEffectLoader is created at load time to facilitate loading
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/p/?LinkId=271568
//--------------------------------------------------------------------------------------

#pragma once

using namespace D3DX11Effects;

#ifdef __cplusplus
EXTERN_C_BEGIN
#endif

NAMESPACE_D3DX11Effects

// Ranges are used for dependency checking during load

enum ERanges
{
	ER_CBuffer = 0,
	ER_Texture,     // Includes TBuffers
	ER_Sampler,
	ER_UnorderedAccessView,
	ER_Interfaces,
	ER_Count        // This should be the size of the enum
};

struct SRange
{
	uint32_t              start;
	uint32_t              last;
	CEffectVector<void *> vResources; // should be (last - start) in length, resource type depends on the range type
};

// Used during load to validate assignments
D3D_SHADER_VARIABLE_TYPE GetSimpleParameterTypeFromObjectType(EObjectType ObjectType);

// A class to facilitate loading an Effect.  This class is a friend of CEffect.
class CEffectLoader
{
	friend EFFECTSAPI HRESULT CEffect::CloneEffect(_In_ uint32_t Flags, _Outptr_ ID3DX11Effect** ppClonedEffect);

protected:
	// changed public:
public:
	// Load-time allocations that eventually get moved happen out of the TempHeap. This heap will grow as needed
	CDataBlockStore             m_BulkHeap;

	uint8_t                     *m_pData;
	SBinaryHeader5              *m_pHeader;
	DWORD                       m_Version;

	CEffect                     *m_pEffect;
	CEffectReflection           *m_pReflection;

	D3DX11Core::CMemoryStream   m_msStructured;
	D3DX11Core::CMemoryStream   m_msUnstructured;

	// used to avoid repeated hash buffer allocations in LoadTypeAndAddToPool
	CEffectVector<uint8_t>      m_HashBuffer;

	uint32_t                    m_dwBufferSize;     // Size of data buffer in bytes

	// List of SInterface blocks created to back class instances bound to shaders
	CEffectVector<SInterface*>  m_BackgroundInterfaces;

	// Pointers to pre-reallocation data
	SGlobalVariable             *m_pOldVars;
	SShaderBlock                *m_pOldShaders;
	SDepthStencilBlock          *m_pOldDS;
	SBlendBlock                 *m_pOldAB;
	SRasterizerBlock            *m_pOldRS;
	SConstantBuffer             *m_pOldCBs;
	SSamplerBlock               *m_pOldSamplers;
	uint32_t                        m_OldInterfaceCount;
	SInterface                  *m_pOldInterfaces;
	SShaderResource             *m_pOldShaderResources;
	SUnorderedAccessView        *m_pOldUnorderedAccessViews;
	SRenderTargetView           *m_pOldRenderTargetViews;
	SDepthStencilView           *m_pOldDepthStencilViews;
	SString                     *m_pOldStrings;
	SMemberDataPointer          *m_pOldMemberDataBlocks;
	CEffectVectorOwner<SMember> *m_pvOldMemberInterfaces;
	SGroup                      *m_pOldGroups;

	uint32_t                    m_EffectMemory;     // Effect private heap
	uint32_t                    m_ReflectionMemory; // Reflection private heap

	// Loader helpers
	EFFECTSAPI HRESULT LoadCBs();
	EFFECTSAPI HRESULT LoadNumericVariable(_In_ SConstantBuffer *pParentCB);
	EFFECTSAPI HRESULT LoadObjectVariables();
	EFFECTSAPI HRESULT LoadInterfaceVariables();

	EFFECTSAPI HRESULT LoadTypeAndAddToPool(_Outptr_ SType **ppType, _In_ uint32_t dwOffset);
	EFFECTSAPI HRESULT LoadStringAndAddToPool(_Outptr_result_maybenull_z_ char **ppString, _In_ uint32_t  dwOffset);
	EFFECTSAPI HRESULT LoadAssignments(_In_ uint32_t Assignments, _Out_writes_(Assignments) SAssignment **pAssignments,
		_In_ uint8_t *pBackingStore, _Out_opt_ uint32_t *pRTVAssignments, _Out_opt_ uint32_t *pFinalAssignments);
	EFFECTSAPI HRESULT LoadGroups();
	EFFECTSAPI HRESULT LoadTechnique(STechnique* pTech);
	EFFECTSAPI HRESULT LoadAnnotations(uint32_t  *pcAnnotations, SAnnotation **ppAnnotations);

	EFFECTSAPI HRESULT ExecuteConstantAssignment(_In_ const SBinaryConstant *pConstant, _Out_writes_bytes_(4) void *pLHS, _In_ D3D_SHADER_VARIABLE_TYPE lhsType);
	EFFECTSAPI uint32_t UnpackData(uint8_t *pDestData, uint8_t *pSrcData, uint32_t PackedDataSize, SType *pType, uint32_t  *pBytesRead);

	// Build shader blocks
	EFFECTSAPI HRESULT ConvertRangesToBindings(SShaderBlock *pShaderBlock, CEffectVector<SRange> *pvRanges);
	EFFECTSAPI HRESULT GrabShaderData(SShaderBlock *pShaderBlock);
	EFFECTSAPI HRESULT BuildShaderBlock(SShaderBlock *pShaderBlock);

	// Memory compactors
	EFFECTSAPI HRESULT InitializeReflectionDataAndMoveStrings(uint32_t KnownSize = 0);
	EFFECTSAPI HRESULT ReallocateReflectionData(bool Cloning = false);
	EFFECTSAPI HRESULT ReallocateEffectData(bool Cloning = false);
	EFFECTSAPI HRESULT ReallocateShaderBlocks();
	template<class T> EFFECTSAPI HRESULT ReallocateBlockAssignments(T* &pBlocks, uint32_t  cBlocks, T* pOldBlocks = nullptr);
	EFFECTSAPI HRESULT ReallocateAnnotationData(uint32_t  cAnnotations, SAnnotation **ppAnnotations);

	EFFECTSAPI HRESULT CalculateAnnotationSize(uint32_t  cAnnotations, SAnnotation *pAnnotations);
	EFFECTSAPI uint32_t  CalculateShaderBlockSize();
	template<class T> EFFECTSAPI uint32_t  CalculateBlockAssignmentSize(T* &pBlocks, uint32_t  cBlocks);

	EFFECTSAPI HRESULT FixupCBPointer(_Inout_ SConstantBuffer **ppCB);
	EFFECTSAPI HRESULT FixupShaderPointer(_Inout_ SShaderBlock **ppShaderBlock);
	EFFECTSAPI HRESULT FixupDSPointer(_Inout_ SDepthStencilBlock **ppDSBlock);
	EFFECTSAPI HRESULT FixupABPointer(_Inout_ SBlendBlock **ppABBlock);
	EFFECTSAPI HRESULT FixupRSPointer(_Inout_ SRasterizerBlock **ppRSBlock);
	EFFECTSAPI HRESULT FixupInterfacePointer(_Inout_ SInterface **ppInterface, _In_ bool CheckBackgroundInterfaces);
	EFFECTSAPI HRESULT FixupShaderResourcePointer(_Inout_ SShaderResource **ppResource);
	EFFECTSAPI HRESULT FixupUnorderedAccessViewPointer(_Inout_ SUnorderedAccessView **ppResource);
	EFFECTSAPI HRESULT FixupRenderTargetViewPointer(_Inout_ SRenderTargetView **ppRenderTargetView);
	EFFECTSAPI HRESULT FixupDepthStencilViewPointer(_Inout_ SDepthStencilView **ppDepthStencilView);
	EFFECTSAPI HRESULT FixupSamplerPointer(_Inout_ SSamplerBlock **ppSampler);
	EFFECTSAPI HRESULT FixupVariablePointer(_Inout_ SGlobalVariable **ppVar);
	EFFECTSAPI HRESULT FixupStringPointer(_Inout_ SString **ppString);
	EFFECTSAPI HRESULT FixupMemberDataPointer(_Inout_ SMemberDataPointer **ppMemberData);
	EFFECTSAPI HRESULT FixupGroupPointer(_Inout_ SGroup **ppGroup);

	// Methods to retrieve data from the unstructured block
	// (these do not make copies; they simply return pointers into the block)
	EFFECTSAPI HRESULT GetStringAndAddToReflection(_In_ uint32_t offset, _Outptr_result_maybenull_z_ char **ppPointer);  // Returns a string from the file string block, updates m_EffectMemory
	EFFECTSAPI HRESULT GetUnstructuredDataBlock(_In_ uint32_t offset, _Out_ uint32_t *pdwSize, _Outptr_result_buffer_(*pdwSize) void **ppData);
	// This function makes a copy of the array of SInterfaceParameters, but not a copy of the strings
	EFFECTSAPI HRESULT GetInterfaceParametersAndAddToReflection(_In_ uint32_t InterfaceCount, _In_ uint32_t offset, _Outptr_result_buffer_all_maybenull_(InterfaceCount) SShaderBlock::SInterfaceParameter **ppInterfaces);
public:

	EFFECTSAPI HRESULT LoadEffect(_In_ CEffect *pEffect, _In_reads_bytes_(cbEffectBuffer) const void *pEffectBuffer, _In_ uint32_t cbEffectBuffer);
};

NAMESPACE_D3DX11Effects_END

#ifdef __cplusplus
EXTERN_C_END
#endif
