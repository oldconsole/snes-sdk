#include <nall/utf8.hpp>

char* realpath(const char *filename, char *resolvedname) {
  wchar_t fn[_MAX_PATH] = L"";
  _wfullpath(fn, nall::utf16_t(filename), _MAX_PATH);
  strcpy(resolvedname, nall::utf8_t(fn));
  return resolvedname;
}

char* userpath(char *path) {
  wchar_t fp[_MAX_PATH] = L"";
  SHGetFolderPathW(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, fp);
  strcpy(path, nall::utf8_t(fp));
  return path;
}

char* getcwd(char *path) {
  wchar_t fp[_MAX_PATH] = L"";
  _wgetcwd(fp, _MAX_PATH);
  strcpy(path, nall::utf8_t(fp));
  return path;
}

void initargs(int &argc, char **&argv) {
  wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
  argv = new char*[argc];
  for(unsigned i = 0; i < argc; i++) {
    argv[i] = new char[_MAX_PATH];
    strcpy(argv[i], nall::utf8_t(wargv[i]));
  }
}

bool Application::App::winEventFilter(MSG *msg, long *result) {
  //supress screen saver from activating during gameplay
  if(msg->message == WM_SYSCOMMAND) {
    if(msg->wParam == SC_SCREENSAVE || msg->wParam == SC_MONITORPOWER) {
      *result = 0;
      return true;
    }
  }

  //prevent DirectSound audio buffer from looping during Windows modal events
  if(msg->message == WM_ENTERMENULOOP || msg->message == WM_ENTERSIZEMOVE) {
    audio.clear();
  }

  return false;
}

void supressScreenSaver() {
  //handled by event filter above
}

