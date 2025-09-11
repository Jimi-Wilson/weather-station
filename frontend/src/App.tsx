import LatestWeatherWidget from "@/components/LatestWeatherWidget.tsx";
import WeatherChartsWidget from "@/components/WeatherChartsWidget.tsx";
import { Navbar } from "@/components/Navbar.tsx";

function App() {
    return (
        <div className="pl-5 pr-5">
            <Navbar />
            <main className="container mx-auto pb-5 pt-5" >
                <div className="flex flex-col gap-4">
                    <LatestWeatherWidget />
                    <WeatherChartsWidget />
                </div>
            </main>
        </div>
    )
}

export default App;