#include "controllers/cli/CLIController.h"
#include "views/cli/CLIView.h"
#include <memory>

auto main() -> int {
    const auto view = std::make_shared<hive::views::cli::CLIView>();
    const hive::controllers::cli::CLIController controller(view);
    controller.run();
    return 0;
}