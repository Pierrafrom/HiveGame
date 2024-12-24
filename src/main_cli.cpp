#include "controllers/cli/CLIController.h"
#include "views/cli/CLIView.h"
#include <memory>
auto main(int argc, char* argv[]) -> int {
    const auto view = std::make_shared<hive::views::cli::CLIView>();
    const hive::controllers::cli::CLIController controller(view);

    std::string saveFilePath;
    if (argc > 1) {
        saveFilePath = argv[1];
    }

    controller.run(saveFilePath);

    return 0;
}
