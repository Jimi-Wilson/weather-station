import { Slider } from "@/components/ui/slider";
import { Label } from "@/components/ui/label";
import { ToggleGroup, ToggleGroupItem } from "@/components/ui/toggle-group";

interface ChartControlsProps {
    viewMode: 'raw' | 'smoothed';
    onViewModeChange: (mode: 'raw' | 'smoothed') => void;
    smoothingIntensity: number;
    onSmoothingIntensityChange: (value: number) => void;
}

export function ChartControls({
                                  viewMode,
                                  onViewModeChange,
                                  smoothingIntensity,
                                  onSmoothingIntensityChange
                              }: ChartControlsProps) {

    const handleValueChange = (value: string) => {
        if (value) {
            onViewModeChange(value as 'raw' | 'smoothed');
        }
    };

    return (
        <div className="sticky top-16 z-10 flex flex-col sm:flex-row gap-4 sm:gap-8 items-center rounded-lg border p-4 mb-4 bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60">
            <ToggleGroup
                type="single"
                variant="outline"
                value={viewMode}
                onValueChange={handleValueChange}
                aria-label="Data view mode"
            >
                <ToggleGroupItem value="smoothed" aria-label="Toggle smoothed data" className="px-5">
                    Smoothed
                </ToggleGroupItem>
                <ToggleGroupItem value="raw" aria-label="Show raw data" className="px-5">
                    Raw
                </ToggleGroupItem>
            </ToggleGroup>

            {viewMode === 'smoothed' && (
                <div className="w-full sm:w-64 flex items-center gap-4">
                    <Label className="whitespace-nowrap">Smoothing Intensity</Label>
                    <Slider
                        value={[smoothingIntensity]}
                        onValueChange={(value) => onSmoothingIntensityChange(value[0])}
                        max={0.99}
                        min={0}
                        step={0.01}
                    />
                    <span className="font-mono text-sm">{smoothingIntensity.toFixed(2)}</span>
                </div>
            )}

        </div>
    );
}