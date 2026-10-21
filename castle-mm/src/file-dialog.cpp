#include "file-dialog.h"

bool openDialog(DialogDesc& dialogDesc, std::filesystem::path& outPath) {
	nfdu8char_t* out{ nullptr };
	nfdresult_t result{ NFD_ERROR };
	if (dialogDesc.folderDialog) {
		nfdpickfolderu8args_t pickFolderDesc{
			.defaultPath = dialogDesc.defaultPath
		};
		result = NFD_PickFolderU8_With(&out, &pickFolderDesc);
	}
	else result = NFD_OpenDialogU8_With(&out, &dialogDesc);
	if (result == NFD_OKAY) {
		outPath = std::filesystem::path{ out };
		NFD_FreePathU8(out);
		return 1;
	}

	return 0;
}
