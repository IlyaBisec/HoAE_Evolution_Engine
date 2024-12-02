#include "TVPlugin.h"
#include "stdint.h"
#include <stdio.h>
#include "kLog.h"

TV_HANDLE *tv_handle = NULL;


TVPLUGIN_API int tv_Init(HWND hWnd)
{
    // TV_HANDLE
    tv_handle = (TV_HANDLE *)malloc(sizeof(TV_HANDLE));
    if (!tv_handle)
	{
		Log.Error("TVPlugin init: Invalid memory allocation or window handle.");
		tv_Done();
        return TV_FAIL;
	}
	ZeroMemory(tv_handle, sizeof(TV_HANDLE));

    // Init DirectSound
	tv_handle->ds = NULL;
	HRESULT hr = DirectSoundCreate(NULL, &tv_handle->ds, NULL);
    if (FAILED(hr))
    {
		Log.Error("TVPlugin init: Failed to initialize DirectSound. HRESULT: 0x%lx", hr);
		SAFE_FREE(tv_handle);
        return TV_FAIL;
	}

	// Get a valid window handle
	if(!hWnd)
	{
		hWnd = GetActiveWindow();
		if(!hWnd)
		{
			hWnd = GetDesktopWindow();
		}
	}
	if(!hWnd)
	{
		Log.Error("TVPlugin init: Failed to get a valid  window handle.");
		SAFE_RELEASE(tv_handle->ds);
        SAFE_FREE(tv_handle);
		return TV_FAIL;
	}

	// Init DirectSound cooperative level 
	 hr = tv_handle->ds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
    if (FAILED(hr))
	{
		Log.Warning("TVPlugin init: Falling back to DSSCL_NORMAL.");
		 hr = tv_handle->ds->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
		 if (FAILED(hr))
		 {
			Log.Error("TVPlugin init: Failed to set DirectSound cooperative level. HRESULT: 0x%lx", hr);
			SAFE_RELEASE(tv_handle->ds);
			SAFE_FREE(tv_handle);
			return TV_FAIL;
		 }
	}
	Log.Info("TVPlugin init: DirectSound initialized successfully.");

	// Check device sound sys
	/*DSCAPS caps = {};
	caps.dwSize = sizeof(DSCAPS);
	hr = tv_handle->ds->GetCaps(&caps);
	if (FAILED(hr)) 
	{
		Log.Error("TVPlugin init: Failed to get device capabilities. HRESULT: 0x%lx", hr);
		return TV_FAIL;
	}
	else
	{
		Log.Info("TVPlugin init: Device capabilities: %s . HRESULT: 0x%lx", caps.dwFlags, hr);
		return TV_OK;
	}*/

    // Init Direct3D
     tv_handle->d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!tv_handle->d3d)
	{
		Log.Error("TVPlugin init: Failed to initialize Direct3D.");
        SAFE_RELEASE(tv_handle->ds);
        SAFE_FREE(tv_handle);
        return TV_FAIL;
	}

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd; // hWnd

	hr = tv_handle->d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &tv_handle->d3dDevice);
    if (FAILED(hr))
    {
		Log.Error("TVPlugin init: Failed to create Direct3D device. HRESULT: 0x%lx", hr);
        SAFE_RELEASE(tv_handle->d3d);
        SAFE_RELEASE(tv_handle->ds);
        SAFE_FREE(tv_handle);
        return TV_FAIL;
    }
	Log.Info("TVPlugin init: Direct3D  initialized successfully.");

    return TV_OK;
}


// Clear resources
void tv_ReleaseHandleResources(TV_HANDLE *handle)
{
    SAFE_RELEASE(handle->dsBuffer);
    SAFE_RELEASE(handle->d3dTexture);
    SAFE_RELEASE(handle->d3dDevice);
    SAFE_RELEASE(handle->d3d);
    if (handle->file) fclose(handle->file);
    SAFE_FREE(handle);
}

TVPLUGIN_API int tv_Done()
{
    // Complete work DirectSound & Direct3D
    if (tv_handle)
    {
       tv_ReleaseHandleResources(tv_handle);
    }

    return TV_OK;
}


TVPLUGIN_API TV_AH tv_StreamOpen(const char *pcszFileName)
{
	// Check open file name
	if (!pcszFileName || !*pcszFileName)
	{
		Log.Error("TVPlugin streamOp: Invalid file name for stream opening.");
		return NULL;
	}

	// Use current tv_handle
	if (!tv_handle || !tv_handle->ds)
	{
		Log.Error("TVPlugin streamOp: DirectSound device is not initialized.");
        return NULL;
    }

	// Create a new stream handle
    TV_AH handle = (TV_AH)malloc(sizeof(TV_HANDLE)); 
	if (!handle)
	{
		Log.Error("TVPlugin streamOp: Failed to allocate memory for stream handle.");
		//SAFE_FREE(handle);
        return NULL;
    }
    ZeroMemory(handle, sizeof(TV_HANDLE));

	// Save DirectSound from global tv_handle
	handle->ds = tv_handle->ds;
	
	// Open ogg vorbis file
    if (ov_fopen(pcszFileName, &handle->vorbisFile) < 0)
    {
		Log.Error("TVPlugin streamOp: Failed to open Ogg Vorbis file.");
        SAFE_FREE(handle);
        return NULL;
    }

    // Create DirectSound buffer
    DSBUFFERDESC dsbd = {};
	//ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
    dsbd.dwBufferBytes = 0;  // 48000 * 2 * 2; 250 ms 16 bit stereo sound on the 48000 Hz
    dsbd.lpwfxFormat = NULL;

	Log.Info("TVPlugin streamOp: Buffer size: %lu bytes for audio stream.", dsbd.dwBufferBytes);

	HRESULT hr = tv_handle->ds->CreateSoundBuffer(&dsbd, &handle->dsBuffer, NULL);
    if (FAILED(hr))
    {
		Log.Error("TVPlugin streamOp: Failed to create DirectSound primary buffer. HRESULT: 0x%08x", hr);
        tv_StreamClose(handle);
        return NULL;
    }

    // Configuring DirectSound Buffer
    WAVEFORMATEX wf = {};
    vorbis_info *vi = ov_info(&handle->vorbisFile, -1);
    wf.wFormatTag = WAVE_FORMAT_PCM;
    wf.nChannels = vi->channels;  // 2 - stereo
    wf.nSamplesPerSec = STREAM_RATE; //vi->rate;
    wf.wBitsPerSample = 16; // default 16
    wf.nBlockAlign = wf.nChannels * (wf.wBitsPerSample / 8);
    wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0; // Additional parameters 

	dsbd.lpwfxFormat = &wf;
	dsbd.dwBufferBytes = wf.nAvgBytesPerSec / 4;  // Buffer for 250 ms(1/4 seconds)

    if (FAILED(handle->dsBuffer->SetFormat(&wf)))
    {
		Log.Error("TVPlugin streamOp: Failed to set format for primary buffer.");
        tv_StreamClose(handle);
        return NULL;
    }
	Log.Info("TVPlugin streamOp: Stream opened successfully.");

    return handle;
}

TVPLUGIN_API int tv_StreamLength(TV_AH tv_ah_handle)
{
    return ov_time_total(&tv_ah_handle->vorbisFile, -1);
}

TVPLUGIN_API void tv_StreamPlay(TV_AH tv_ah_handle)
{
	if (!tv_ah_handle || !tv_ah_handle->dsBuffer)
	{
		Log.Error("TVPlugin streamPlay: Invalid tv_ah_handle or buffer.");
        return;
    }

    // Read data from vorbisFile & write to DirectSound buffer
    vorbis_info *vi = ov_info(&tv_ah_handle->vorbisFile, -1);
    char buffer[STREAM_BUFFER_SIZE];
    long  bytesRead =  ov_read(&tv_ah_handle->vorbisFile, buffer,
        sizeof(buffer), 0, 2, 1, NULL);
  
	if (bytesRead <= 0) 
	{
        Log.Error("TVPlugin streamPlay: Failed to read audio data from Ogg Vorbis file.");
        return;
	}

	 DWORD dwWriteOffset = 0;

    while (bytesRead > 0)
    {
        void *pDSLockedBuffer = NULL;
        DWORD dwDSLockedBufferSize = 0;

        // Blocking part of DirectSound for write
		HRESULT hr = tv_ah_handle->dsBuffer->Lock(dwWriteOffset, bytesRead, &pDSLockedBuffer,
            &dwDSLockedBufferSize, NULL, NULL, 0);
        if (FAILED(hr))
        {
			Log.Error("TVPlugin streamPlay: Failed to lock DirectSound buffer. HRESULT: 0x%lx", hr);
            return;
        }

        // Copying data Vorbis to DirectSound buffer
        memcpy(pDSLockedBuffer, buffer, dwDSLockedBufferSize);

		hr = tv_ah_handle->dsBuffer->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);
        // Unlocking DirectSound buffer after write
		if (FAILED(hr))
        {
			Log.Error("TVPlugin streamPlay: Failed to unlock DirectSound buffer. HRESULT: 0x%lx", hr);
            return;
        }
        dwWriteOffset += bytesRead;
    }

	HRESULT hr = tv_ah_handle->dsBuffer->Play(0, 0, 0);
    if (FAILED(hr))
    {
		Log.Error("TVPlugin streamPlay: Failed to start playback. HRESULT: 0x%lx", hr);
        return;
    }
}


TVPLUGIN_API void tv_StreamClose(TV_AH tv_ah_handle)
{
    if (tv_ah_handle)
    {
         ov_clear(&tv_ah_handle->vorbisFile);
		 tv_ReleaseHandleResources(tv_ah_handle);
    }
}

TVPLUGIN_API BYTE tv_StreamFinished(TV_AH tv_ah_handle)
{
    return ov_time_tell(&tv_ah_handle->vorbisFile) >= ov_time_total(&tv_ah_handle->vorbisFile, -1);
}

TVPLUGIN_API void tv_StreamSetVolume(TV_AH tv_ah_handle, TV_VOLUME tv_volume)
{
    // Setting the volume of the audio stream
    if (tv_ah_handle->dsBuffer)
    {
        // Converting the volume value from the range [0, 100] to the DirectSound range [-10000, 0]
        LONG volume = TV_DSBVOLUME_MIN + (tv_volume * (TV_DSBVOLUME_MAX - TV_DSBVOLUME_MIN)) / TV_VOLUME_MAX;
        tv_ah_handle->dsBuffer->SetVolume(volume);
    }
   //LONG dsVolume = DSBVOLUME_MIN + ((tv_volume - TV_VOLUME_MIN) * (DSBVOLUME_MAX - DSBVOLUME_MIN)) / (TV_VOLUME_MAX - TV_VOLUME_MIN);
}

TVPLUGIN_API TV_VH tv_VideoOpen(const char *pcszFileName)
{
    TV_VH handle = (TV_VH)malloc(sizeof(TV_HANDLE));
	if (!handle) return NULL;

    handle->file = fopen(pcszFileName, "rb");
    if (!handle->file)
    {
		Log.Error("TVPlugin videoOp: Failed, can not open file.");
        free(handle);
        return NULL;
    }

    // Init Ogg & Theora
    ogg_sync_init(&handle->oggSyncState);
    ogg_stream_init(&handle->theoraStreamState, 0);
    ogg_stream_init(&handle->vorbisStreamState, 0);

    // Read file and init Theora
    char *buffer = ogg_sync_buffer(&handle->oggSyncState, 4096);
    int bytes = fread(buffer, 1, 4096, handle->file);
    ogg_sync_wrote(&handle->oggSyncState, bytes);

    ogg_page oggPage;
    if (ogg_sync_pageout(&handle->oggSyncState, &oggPage) != 1)
    {
		tv_VideoClose(handle);
        return NULL;
    }

    ogg_stream_pagein(&handle->theoraStreamState, &oggPage);
    ogg_packet oggPacket;

    if (ogg_stream_packetout(&handle->theoraStreamState, &oggPacket) != 1)
    {
	    tv_VideoClose(handle);
        return NULL;
    }

    th_info theoraInfo;
    th_comment theoraComment;
    th_info_init(&theoraInfo);
    th_comment_init(&theoraComment);

    if (th_decode_headerin(&theoraInfo, &theoraComment,
        &handle->theoraSetup, &oggPacket) < 0)
    {
        tv_VideoClose(handle);
        return NULL;
    }

    handle->theoraDecoder = th_decode_alloc(&theoraInfo, handle->theoraSetup);
    th_info_clear(&theoraInfo);
    th_comment_clear(&theoraComment);

    // Init Direcr3D texture
	HRESULT hr = tv_handle->d3dDevice->CreateTexture(
        theoraInfo.frame_width,
        theoraInfo.frame_height,
        1,
        D3DUSAGE_DYNAMIC,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &handle->d3dTexture,
        NULL
		);

    if (FAILED(hr))
    {
		Log.Error("TVPlugin videoOp: Failed to create Direct3D texture.");
        tv_VideoClose(handle);
        return NULL;
    }

    return handle;
}


TVPLUGIN_API int tv_VideoClose(TV_VH tv_vh_handle)
{
	if (!tv_vh_handle) return TV_FAIL;

    fclose(tv_vh_handle->file);
    ogg_sync_clear(&tv_vh_handle->oggSyncState);
    ogg_stream_clear(&tv_vh_handle->theoraStreamState);
    ogg_stream_clear(&tv_vh_handle->vorbisStreamState);

    if (tv_vh_handle->d3dTexture)
    {
        tv_vh_handle->d3dTexture->Release();
    }
    th_decode_free(tv_vh_handle->theoraDecoder);
    free(tv_vh_handle);

    return TV_OK;
}

TVPLUGIN_API int tv_VideoFinished(TV_VH tv_vh_handle)
{
    return feof(tv_vh_handle->file) ? TV_OK : TV_FAIL;
}

TVPLUGIN_API void tv_VideoPlay(TV_VH tv_vh_handle, BOOL bLooped, RECT *pRect)
{
	if (!tv_vh_handle || tv_vh_handle->d3dTexture) 
	{
		Log.Error("TVPlugin videoPlay: Invalid video handle or texture.");
        return;
	}

    while (true)
    {
        ogg_packet oggPacket;
        while (ogg_stream_packetout(&tv_vh_handle->theoraStreamState, &oggPacket) > 0)
        {
            if (th_decode_packetin(tv_vh_handle->theoraDecoder, &oggPacket, NULL) == 0)
            {
                th_ycbcr_buffer ycbcrBuffer;
                th_decode_ycbcr_out(tv_vh_handle->theoraDecoder, ycbcrBuffer);

                // Draw decoding frame on the Direct3D texture 
                D3DLOCKED_RECT lockedRect;																				// 0
				HRESULT hr = tv_vh_handle->d3dTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
                if (SUCCEEDED(hr))
                {
					Log.Info("TVPlugin videoPlay: Success to lock Direct3D texture. HRESULT: 0x%lx", hr);
                    for (int y = 0; y < ycbcrBuffer[0].height; y++)
                    {
						uint8_t *srcY = ycbcrBuffer[0].data + y * ycbcrBuffer[0].stride;
                        uint32_t *dst = (uint32_t *)((BYTE *)lockedRect.pBits + y * lockedRect.Pitch);

                        for (int x = 0; x < ycbcrBuffer[0].width; x++)
                        {
							int y_value = srcY[x];
							int u_value = ycbcrBuffer[1].data[(y / 2) * ycbcrBuffer[1].stride + (x / 2)];
                            int v_value = ycbcrBuffer[2].data[(y / 2) * ycbcrBuffer[2].stride + (x / 2)];

                            int c = y_value - 16;
                            int d = u_value - 128;
                            int e = v_value - 128;

                            int r = (298 * c + 409 * e + 128) >> 8;
                            int g = (298 * c - 100 * d - 208 * e + 128) >> 8;
                            int b = (298 * c + 516 * d + 128) >> 8;

                            r = (r < 0) ? 0 : (r > 255) ? 255 : r;
                            g = (g < 0) ? 0 : (g > 255) ? 255 : g;
                            b = (b < 0) ? 0 : (b > 255) ? 255 : b;

							dst[x] = D3DCOLOR_XRGB(r, g, b);

                          //  ((DWORD *)lockedRect.pBits)[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_XRGB(r, g, b);
                        }
                    }
                    tv_vh_handle->d3dTexture->UnlockRect(0);
                }

                // Rendering  texture on the screen
                tv_vh_handle->d3dDevice->BeginScene();
                tv_vh_handle->d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

				// Get surface from texture
				IDirect3DSurface9* pSurface = NULL;
				if (FAILED(tv_vh_handle->d3dTexture->GetSurfaceLevel(0, &pSurface)))
				{
					Log.Error("TVPlugin videoPlay: Failed to get surface from texture.");
					return;
				}

				tv_vh_handle->d3dDevice->StretchRect(
                    pSurface,
                    NULL,
                    NULL,
                    pRect,
                    D3DTEXF_LINEAR
                );
				
				pSurface->Release();
				tv_vh_handle->d3dDevice->EndScene();
                tv_vh_handle->d3dDevice->Present(NULL, NULL, NULL, NULL);

            }
        }

        // Read the next ogg page
        char *buffer = ogg_sync_buffer(&tv_vh_handle->oggSyncState, 4096);
        int bytes = fread(buffer, 1, 4096, tv_vh_handle->file);
        if (bytes == 0) break;

        ogg_sync_wrote(&tv_vh_handle->oggSyncState, bytes);
        while (ogg_sync_pageout(&tv_vh_handle->oggSyncState, &tv_vh_handle->oggPage) > 0)
        {
            ogg_stream_pagein(&tv_vh_handle->theoraStreamState, &tv_vh_handle->oggPage);
        }

        if (bLooped && feof(tv_vh_handle->file))
        {
            fseek(tv_vh_handle->file, 0, SEEK_SET);
            ogg_sync_reset(&tv_vh_handle->oggSyncState);
           // ogg_stream_reset(&tv_vh_handle->theoraStreamState);
          //  ogg_stream_reset(&tv_vh_handle->vorbisStreamState);
        }
    }
	Log.Info("TVPlugin videoPlay: Video playback finished.");
}

TVPLUGIN_API void tv_VideoResize(TV_VH tv_vh_handle, RECT *pRect)
{
	if (!tv_vh_handle || !pRect) return;

	// Calculate a new texture size
	int newWidth = pRect->right - pRect->left;
    int newHeight = pRect->bottom - pRect->top;	
	
	// Check correct size 
	if (newWidth <= 0 || newHeight <= 0) return;

	 // Change size texture for the new window size
	D3DSURFACE_DESC desc;
	if (tv_vh_handle->d3dTexture)
	{
		 tv_vh_handle->d3dTexture->GetLevelDesc(0, &desc);

		 // If size not changed
		if (desc.Width == newWidth && desc.Height == newHeight)
		{
			return;
		}
		// Delete old texture
		tv_vh_handle->d3dTexture->Release();
        tv_vh_handle->d3dTexture = NULL;
	}
   
	// Create a new texture with updated size
    HRESULT hr  = tv_vh_handle->d3dDevice->CreateTexture(
		newWidth,
		newHeight,
        1,
        D3DUSAGE_DYNAMIC,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &tv_vh_handle->d3dTexture,
        NULL
    );

	if (FAILED(hr))
	{
		Log.Error("TVPlugin videoResize: Failed to create texture with new size: %dx%d (HRESULT: 0x%08X)\n", newWidth, newHeight, hr);
		tv_vh_handle->d3dTexture = NULL;
	}
}