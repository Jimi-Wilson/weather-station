import LatestWeatherWidget from "@/components/LatestWeatherWidget.tsx";
import WeatherChartsWidget from "@/components/WeatherChartsWidget.tsx";


function App() {
    return (
        <div className={"container mx-auto p-4"}>
            <div className="flex flex-col gap-4 bg-background">
                <LatestWeatherWidget />
                <WeatherChartsWidget />
            </div>
        </div>
    )
}

export default App;